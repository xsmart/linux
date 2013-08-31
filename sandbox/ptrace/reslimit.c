#include <unistd.h>
#include <pthread.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>     /* For SIGKILL */
#include <sys/stat.h>   /* For stat */

#include "reslimit.h"
#include "common.h"
#include "config.h"

// page size is 4KB in 32-bits Linux
#define PAGE_SIZE_32_BITS 4

// Read 24 columns from file /proc/<pid>/stat,
// so we can get child's resource usage in real time
#define PROC_RES_NUM 24
// user time is the 14th column in /proc/<pid>/stat
#define USER_TIME_INDEX 13
#define SYSTEM_TIME_INDEX 14
#define MEMORY_INDEX 23

// Thread reads file /proc/<pid>/stat every 50ms (50000us)
#define THREAD_SLEEP_TIME 50000

static struct conf_t* s_conf_info = NULL;

int reslimit(struct conf_t* conf_info, pid_t* pid)
{
    s_conf_info = conf_info;

    pthread_t rlimit_thread;
    int res = pthread_create(&rlimit_thread, NULL, thread_reslimit, (void*)pid);
    if(res != 0)
    {
        fprintf(stderr, "Failed to create resource limit thread\n");
        return RET_ERR;
    }

    res = pthread_detach(rlimit_thread);
    if(res != 0)
    {
        fprintf(stderr, "Detach resource limit thread error\n");
        return RET_ERR;
    }
    return RET_OK;
}

int is_cpu_exceeding(const struct res_usage_t* res_usage,
                     const struct conf_t* conf_info)
{
    if(res_usage->elapsed_time >= conf_info->cputime * 1000)
    {
        return RET_TRUE;
    }
    return RET_FALSE;
}

int is_mem_exceeding(const struct res_usage_t* res_usage,
                     const struct conf_t* conf_info)
{
    if(res_usage->max_rss >= conf_info->memory)
    {
        return RET_TRUE;
    }
    return RET_FALSE;
}

int is_fsize_exceeding(const struct conf_t* conf_info)
{
    struct stat file_stat;
    if(stat(conf_info->file_out, &file_stat) == 0
    && file_stat.st_size >= conf_info->file_size * UNIT_FSIZE)
    {
        return RET_TRUE; 
    }

    if(stat(conf_info->file_err, &file_stat) == 0
    && file_stat.st_size >= conf_info->file_size * UNIT_FSIZE)
    {
        return RET_TRUE; 
    }
    return RET_FALSE;
}

int is_res_exceeding(struct res_usage_t* res_usage, struct conf_t* conf_info)
{
    if(is_cpu_exceeding(res_usage, conf_info) == RET_TRUE
    || is_mem_exceeding(res_usage, conf_info) == RET_TRUE
    || is_fsize_exceeding(conf_info) == RET_TRUE)
    {
        return RET_TRUE;
    }
    return RET_FALSE;
}


static void* thread_reslimit(void* arg)
{
    pid_t* child_pid = (pid_t*)arg;
    char str_rusage[256];
    memset(str_rusage, 0, sizeof(str_rusage));
    
    char proc_info_file[MAX_PATH];
    sprintf(proc_info_file, "/proc/%d/stat", *child_pid);

    // If child process has exited already,
    // /proc/<pid>/stat will be non existent,
    // and fp will be NULL.
    FILE* fp = NULL;
    while(fp = fopen(proc_info_file, "r"))
    {
        struct res_usage_t res_usage;
        res_usage.elapsed_time = 0;
        res_usage.max_rss = 0;

        int i = 0;
        while(i < PROC_RES_NUM && fscanf(fp, "%s", str_rusage))
        {
            if(i == USER_TIME_INDEX || i == SYSTEM_TIME_INDEX)
            {
                res_usage.elapsed_time += atoi(str_rusage) * 10;
            }
            else if(i == MEMORY_INDEX)
            {
                res_usage.max_rss = atoi(str_rusage) * PAGE_SIZE_32_BITS;
            }
            i ++;
        }

        if(is_res_exceeding(&res_usage, s_conf_info) == RET_TRUE)
        {
            if(kill(*child_pid, SIGKILL) == -1)
                fprintf(stderr, "Child process has exited already\n");
        }
        fclose(fp);
        usleep(THREAD_SLEEP_TIME);
    }

    fprintf(stderr, "Child process has exited already\n");
    pthread_exit(NULL); 
    return NULL;
}


int get_proc_result(const struct conf_t* conf_info,
                    const struct res_usage_t* res_usage)
{
     if(is_cpu_exceeding(res_usage, conf_info) == RET_TRUE)
     {
        return RESULT_TL;
     }

     if(is_mem_exceeding(res_usage, conf_info) == RET_TRUE)
     {
        return RESULT_ML;
     }

     if(is_fsize_exceeding(conf_info) == RET_TRUE)
     {
        return RESULT_OL;
     }

     return RESULT_OK;
}
