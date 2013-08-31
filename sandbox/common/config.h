#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_PATH 256
// max number of the files that the sandboxed process can access
#define MAX_FILE_NUMBER 50
// access flag: r or w(which represents file can be read or written)
#define ACCESS_FLAG_LENGTH 4

#ifdef __cplusplus
extern "C"
{
#endif

enum unit_t
{
    UNIT_CPU = 1,       /* 1 S */
    UNIT_MEM = 1024,    /* 1024 KB */
    UNIT_FSIZE = 1024,  /* 1024 KB */
};

struct file_list_t
{
    char file[MAX_PATH];
    char access_flag[ACCESS_FLAG_LENGTH];
};

struct conf_t
{
    char file_name[MAX_PATH];
    int cputime;
    int memory;
    int file_size;

    char file_in[MAX_PATH];
    char file_out[MAX_PATH];
    char file_err[MAX_PATH];

    char sandbox_opt[MAX_PATH];

    int file_num;
    struct file_list_t file_list[MAX_FILE_NUMBER];
};

int read_conf(const char* file_name,struct conf_t* conf_info);

static void read_file_list(FILE* fd,struct conf_t* conf_info);

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif  /* __CONFIG_H__ */
