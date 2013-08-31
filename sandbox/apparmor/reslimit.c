/*****************************************************************************
 *        File: reslimit.c
 * Description: use syscall setrlimit to implement resource control
 *      Author: Lei              
 *****************************************************************************/
#include "reslimit.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "config.h"
#include "common.h"

#define READ_FLAG O_RDONLY
#define WRITE_FLAG O_WRONLY|O_TRUNC

static void
init_rlimit(struct rlimit* resource,const int softlimit,
            const int hardlimit,enum unit_t unit)
{
    resource->rlim_cur = softlimit*unit;
    resource->rlim_max= hardlimit*unit;
}            

static int set_res_limit(int resource,const int softlimit,
                         const int hardlimit,enum unit_t unit)
{
    struct rlimit rlimit_t;
    init_rlimit(&rlimit_t,softlimit,hardlimit,unit);
    return setrlimit(resource,&rlimit_t);
}

static int execute_rlimit(const struct conf_t* conf_info)
{
    if(set_res_limit(RLIMIT_CPU,conf_info->cputime,RLIM_INFINITY,UNIT_CPU) == -1
     ||set_res_limit(RLIMIT_RSS,conf_info->memory,conf_info->memory,UNIT_MEM) == -1
     ||set_res_limit(RLIMIT_FSIZE,conf_info->file_size,conf_info->file_size,UNIT_FSIZE) == -1)
    {
        fprintf(stderr,"Failed to set resource for child process");
        return RET_ERR;
    }

    return RET_OK;
}

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

int execute_configuration(const struct conf_t* conf_info)
{
    if(execute_rlimit(conf_info) == -1
     ||execute_stream_redirection(conf_info) == -1)
    {
        return RET_ERR;
    }

    return RET_OK;
}

