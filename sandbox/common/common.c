/*****************************************************************************
 *        File: common.h
 * Description: common define for sandbox
 *      Author: Lei              
 *****************************************************************************/
#include "common.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int redirect_stream(const char* file,int streamfd,const int option)
{
    int fd;
    fd=open(file,option);
    if(fd == -1)
    {
        fprintf(stderr,"Failed to open file: %s",file);
        return RET_ERR;
    }

    if(dup2(fd,streamfd) == -1)
    {
        fprintf(stderr,"Failed to redirect streamfd: %d",streamfd);
        return RET_ERR;
    }

    if(close(fd) == -1)
    {
        fprintf(stderr,"Failed to close file: %s",file);
        return RET_ERR;
    }

    return RET_OK;
}


int get_file_name(const char* path, char* file_name)
{
    if(path == NULL || file_name == NULL)
    {
        fprintf(stderr,"get_file_name parameter err: path or file_name can't be NULL\n");
        return RET_ERR;
    }

    int length = strlen(path);
    int position = length - 1;

    while(position > 0 && path[position] != '/')
        position --;

    if(position == 0)
        strcpy(file_name, path);    // there is no '/' in path
    else
        strcpy(file_name, path + position + 1);// file name is after '/'

    return RET_OK;
}


void get_resusage(struct res_usage_t* res_usage, struct rusage proc_resusage)
{
    res_usage->elapsed_time = (proc_resusage.ru_utime.tv_sec +
                               proc_resusage.ru_stime.tv_sec)*1000;

    res_usage->elapsed_time += (double)(proc_resusage.ru_utime.tv_usec +
                                        proc_resusage.ru_stime.tv_usec)/1000;
        
    res_usage->max_rss = proc_resusage.ru_maxrss;
}
