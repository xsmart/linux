/*****************************************************************************
 *        File: ptrace.c
 * Description: use system call ptrace to trace child process, so parent process
                can know what files child process open
 *      Author: Lei              
 *****************************************************************************/
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/reg.h>       /* For ORIG_EAX etc */
#include <sys/syscall.h>   /* For SYS_open etc */

#include "common.h"
#include "config.h"
#include "ptrace.h"
#include "file_access.h"

#define READ_FLAG O_RDONLY
#define WRITE_FLAG O_WRONLY|O_TRUNC

static int execute_stream_redirection(const struct conf_t* conf_info)
{
    if(redirect_stream(conf_info->file_in,STDIN_FILENO,READ_FLAG) == -1
     ||redirect_stream(conf_info->file_out,STDOUT_FILENO,WRITE_FLAG) == -1
     ||redirect_stream(conf_info->file_err,STDERR_FILENO,WRITE_FLAG) == -1)
    {
        return RET_ERR;
    }
    return RET_OK;
}

static int ptrace_sandbox_init(const struct conf_t* conf_info)
{
    uid_t uid = getuid();
    if(setuid(uid) == -1)
    {
        fprintf(stderr,"Failed to change effective uid\n");
        return RET_ERR;
    }

    return execute_stream_redirection(conf_info);
}

int start_ptrace_sandbox(const struct conf_t* conf_info, struct res_usage_t* res_usage)
{
    pid_t child_pid = fork();
    if(child_pid == -1)
    {
        fprintf(stderr, "Failed to fork\n");
        return RESULT_IE;
    }

    if(child_pid == 0)
    {
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
        {
            fprintf(stderr, "Trace child process error\n");
            return RESULT_IE;
        }

        char file_name[MAX_PATH];
        if(get_file_name(conf_info->file_name, file_name) == RET_OK
        && ptrace_sandbox_init(conf_info) == RET_OK)
        {
            execl(conf_info->file_name,file_name,NULL);
        }

        fprintf(stderr, "Child process failed to exec\n");
        return RESULT_IE;
    }
    else
    {
        reslimit(conf_info, &child_pid);
        return wait_child_proc(child_pid, conf_info, res_usage);
    }
}

static int is_sysopen(pid_t child_pid)
{
    long eax = ptrace(PTRACE_PEEKUSER, child_pid, 4 * ORIG_EAX, NULL);
    //printf("System ID is %ld\n", eax);
    return eax == SYS_open;
}

static int trace_child_syscall(pid_t child_pid, const struct conf_t* conf_info, int* kill_child)
{
    if(is_sysopen(child_pid)
    && check_open_file(child_pid, conf_info) == RET_ERR)
    {
        if(ptrace(PTRACE_KILL, child_pid, NULL, NULL) == -1)
        {
            fprintf(stderr, "Kill child process error\n");
            return RET_ERR;
        }
        *kill_child = 1;
    }
    return RET_OK;
}

static int wait_child_proc(pid_t child_pid, const struct conf_t* conf_info,
                           struct res_usage_t* res_usage)
{
    int status = 0;
    int is_syscall_entry = 1;
    int kill_child = 0;

    struct rusage child_rusage;
    do
    {
        if(wait4(child_pid, &status, 0, &child_rusage) < 0)
        {
            fprintf(stderr, "Waitpid error\n");
            return RESULT_IE;
        }

        if(WIFEXITED(status) || WIFSIGNALED(status))
        { 
            get_resusage(res_usage, child_rusage);
            printf("Child process exit\n");
            return get_proc_result(conf_info, res_usage);
        }
        else if(WIFSTOPPED(status))
        {
            if(is_syscall_entry && is_sysopen(child_pid))
            {
                trace_child_syscall(child_pid, conf_info, &kill_child);
            }
            is_syscall_entry = 1 & ~is_syscall_entry;

            if(!kill_child && ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
            {
                fprintf(stderr, "Trace system call error\n");
                return RET_ERR;
            }
        }
    }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    return RESULT_OK;
}
