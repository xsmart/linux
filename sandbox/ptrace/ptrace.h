/*****************************************************************************
 *        File: ptrace.h
 * Description: use system call ptrace to trace child process, so parent process
                can know what files child process open
 *      Author: Lei              
 *****************************************************************************/
#ifndef __PTRACE_H__
#define __PTRACE_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct conf_t;
struct res_usage_t;

static int execute_stream_redirection(const struct conf_t* conf_info);

static int ptrace_sandbox_init(const struct conf_t* conf_info);

static int wait_child_proc(pid_t child_pid, const struct conf_t* conf_info,
                           struct res_usage_t* child_rusage);

int start_ptrace_sandbox(const struct conf_t* conf_info, struct res_usage_t* res_usage); 

static int is_sysopen(pid_t child_pid);

/**
 * When child process opens a file(it will call sysopen),parent can check what file child opens,
 * if there is no access permission for child,parent will kill it.
 */
static int trace_child_syscall(pid_t child_pid, const struct conf_t* conf_info, int* kill_child);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __PTRACE_H__ */
