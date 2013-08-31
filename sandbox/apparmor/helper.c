/*****************************************************************************
 *        File: helper.c
 * Description: use apparmor to execute file system access control,and
 *              start a apparmor helper process to execute resource limit
 *      Author: Lei              
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include "helper.h"
#include "reslimit.h"
#include "config.h"
#include "common.h"

int apparmor_init(const struct conf_t* conf_info)
{
    char profile_name[MAX_PATH];
    get_profile_name(conf_info->file_name, profile_name);

    if(write_profile(profile_name, conf_info) < 0)
    {
        fprintf(stderr,"Write profile error\n");
        return -1;
    }
    // reload apparmor profile
    if(system("/etc/init.d/apparmor reload") == -1)
    {
        fprintf(stderr,"Failed to reload apparmor profile\n");
        return RET_ERR;
    }

    // Now the effective uid is root,change effective uid to real uid
    // after create apparmor profile
    uid_t uid = getuid();
    if(seteuid(uid) < 0)
    {
        fprintf(stderr,"Failed to change effective uid\n");
        return -1;
    }
    return RET_OK;
}

int apparmor_fini(const struct conf_t *conf_info)
{
    // Set effective uid root,so we can delete apparmor profile
    if(setuid(0) < 0)
    {
        fprintf(stderr,"Failed to change effective uid to root.\n");
        return RET_ERR;
    }

    char profile_name[MAX_PATH];
    get_profile_name(conf_info->file_name, profile_name);
    if(remove(profile_name) != 0)
    {
        fprintf(stderr,"Failed to delete apparmor profile.\n");
        return RET_ERR;
    }
    return RET_OK;
}


/**
 * here is a sample apparmor profile's name:
 * home.lei.sandbox.demoexe
 * so we need to replace '/' with '.' to get profile's name
 * (ignore first '/' in path)
 */
static void get_profile_name(const char* path, char* prof_name)
{
    char buffer[MAX_PATH];

    int count = 0;
    while(*path != '\0')
    {
        if('/' == *path && 0 != count)
        {
            buffer[count++] = '.';   
        }
        else if('/' != *path)
        {
            buffer[count++] = *path;
        }
        path ++;
    }
    buffer[count] = '\0';
    sprintf(prof_name, "/etc/apparmor.d/%s", buffer);
}

/**
 * Here is sample profile:
 * #include <tunables/global>                                                  
 * /home/lei/apparmor-helper/demoexe {
 *    #include <abstractions/base>
 *    
 *    /home/lei/data rw,
 * }
 */
int write_profile(const char* prof_path, const struct conf_t* conf_info)
{
    FILE* fd;
    fd = fopen(prof_path, "w+");

    if(NULL == fd)
    {
        fprintf(stderr,"Failed to open file: %s\n",prof_path);
        return RET_ERR;
    }

    fprintf(fd,"#include <tunables/global>\n");
    fprintf(fd,"%s {\n",conf_info->file_name);
    fprintf(fd,"\t#include <abstractions/base>\n");

    int i;
    for(i=0; i<conf_info->file_num; i++)
    {
        fprintf(fd,"\t%s %s,\n",conf_info->file_list[i].file,
                                conf_info->file_list[i].access_flag);
    }
    fprintf(fd,"}\n");

    if(0 != fclose(fd))
    {
        fprintf(stderr,"Failed to close file: %s\n",prof_path);
        return RET_ERR;
    }
    return RET_OK;
}

static int get_proc_exit_stat(int signal)
{
    switch(signal)
    {
        case SIGXCPU:
            return RESULT_TL;
        case SIGSEGV:
            return RESULT_ML;
        case SIGXFSZ:
            return RESULT_OL;
        default:
            return RESULT_IE;
    } 
}

int get_result(const pid_t childpid,struct res_usage_t* res_usage)
{
    //the status of child process
    int status = 0;

    struct rusage child_rusage;
    //wait for child process to exit and it's resource usage
    if(wait4(childpid,&status,0,&child_rusage) < 0)
    {
        fprintf(stderr,"Failed to wait for child process\n");
        return RESULT_IE;
    }

    get_resusage(res_usage, child_rusage);

    //if child process terminated normally
    if(WIFEXITED(status))
        return RESULT_OK;

    //if child process was terminated by a signal
    if(WIFSIGNALED(status))
        return get_proc_exit_stat(WTERMSIG(status));
    else
    {
        fprintf(stderr, "Child process exited abnormally\n");
        return RESULT_IE;
    }
}


int start_apparmor_helper(const struct conf_t* conf_info,struct res_usage_t* res_usage)
{
    if(apparmor_init(conf_info) == RET_ERR)
    {
       fprintf(stderr,"apparmor_init error\n");
       return RESULT_IE;
    }

    pid_t childpid = fork();
    if(childpid == -1)
    {
        fprintf(stderr,"Failed to fork\n");
        return RESULT_IE;
    }

    if(childpid == 0)
    {
        //If set resource limit successfully,call execl;
        char file_name[MAX_PATH];
        if(get_file_name(conf_info->file_name,file_name) == RET_OK
        && execute_configuration(conf_info) == RET_OK)
        {
            execl(conf_info->file_name,file_name,NULL);
            fprintf(stderr,"Child process failed to exec\n");
        }
        return RESULT_IE;
    }
    else
    {
        int result = get_result(childpid,res_usage);
        apparmor_fini(conf_info);
        return result;
    }   
}
