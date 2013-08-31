/*****************************************************************************
 *        File: file_access.h
 * Description: Check what files child process opened for ptrace sandbox
 *      Author: Lei              
 *****************************************************************************/
#ifndef __FILE_ACCESS_H__
#define __FILE_ACCESS_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct conf_t;
struct res_usage_t;

/**
 * Get file name and open flag from child process's memory
 */
static int get_open_file(pid_t child_pid, const struct conf_t* conf_info,
                         char* file_name, int* open_flag);

/**
 * Check whether the file child opened can match files in configuration file
 */
static int check_file_access(const struct conf_t* conf_info,
                             char* file_name, int open_flag);

int check_open_file(pid_t child_pid, const struct conf_t* conf_info);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __FILE_ACCESS_H__ */
