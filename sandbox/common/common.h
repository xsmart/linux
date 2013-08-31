/*****************************************************************************
 *        File: common.h
 * Description: common define for sandbox
 *      Author: Lei              
 *****************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <sys/resource.h>

enum return_t
{
    RET_OK = 0,
    RET_ERR = -1,
    RET_TRUE = 1,
    RET_FALSE = 0
};

enum result_t
{
    RESULT_OK = 0,  /* Okay */
    RESULT_TL = 1,  /* Time Limit Exceed */
    RESULT_ML = 2,  /* Memory Limit Exceed */
    RESULT_OL = 3,  /* Output Limit Exceed */
    RESULT_IE = 4,  /* Internal Error */
};

struct res_usage_t
{
   long elapsed_time;
   long max_rss;
};

/**
 * redirect stdin,stdout or stderror stream
 */
int redirect_stream(const char* file,int streamfd,const int option);

/**
 * get file name from a path
 */
int get_file_name(const char* path, char* file_name);

void get_resusage(struct res_usage_t* res_usage, struct rusage proc_resusage);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __COMMON_H__ */
