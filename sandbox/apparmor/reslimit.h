/*****************************************************************************
 *        File: reslimit.h
 * Description: use syscall setrlimit to implement resource control
 *      Author: Lei              
 *****************************************************************************/
#ifndef __RESLIMIT_H__
#define __RESLIMIT_H__

#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct conf_t;
enum unit_t;

static void init_rlimit(struct rlimit* resource,const int softlimit,
                        const int hardlimit,enum unit_t unit);

static int set_res_limit(int resource,const int softlimit,
                         const int hardlimit,enum unit_t unit);

static int execute_rlimit(const struct conf_t* conf_info);

static int execute_stream_redirection(const struct conf_t* conf_info);

int execute_configuration(const struct conf_t* conf_info);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __RESRLIMIT_H__ */
