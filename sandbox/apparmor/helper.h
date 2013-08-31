/*****************************************************************************
 *        File: helper.h
 * Description: use apparmor to execute file system access control,and
 *              start a apparmor helper process to execute resource limit
 *      Author: Lei              
 *****************************************************************************/
#ifndef __HELPER_H__
#define __HELPER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <unistd.h>

struct conf_t;
struct res_usage_t;

int apparmor_init(const struct conf_t* conf_info);

/**
 * delete apparmor profile, change process's euid
 */
int apparmor_fini(const struct conf_t * conf_info);

static int get_proc_exit_stat(int signal);

int get_result(const pid_t childpid,struct res_usage_t* res_usage);

/**
 * Replace '/' with '.' in path to get profile's name
 * (ignore first '/' in path)
 */
static void get_profile_name(const char* path, char* prof_name);

/**
 * Write file list and some basic content to apparmor profile 
 */
static int write_profile(const char* prof_path, const struct conf_t* conf_info);

int start_apparmor_helper(const struct conf_t* conf_info,struct res_usage_t* res_usage);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __HELPER_H__ */
