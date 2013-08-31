#ifndef __RESLIMIT_H__
#define __RESLIMIT_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct res_usage_t;
struct conf_t;

int reslimit(struct conf_t* conf_info, pid_t* pid);

static int is_cpu_exceeding(const struct res_usage_t* res_usage,
                            const struct conf_t* conf_info);

static int is_mem_exceeding(const struct res_usage_t* res_usage,
                            const struct conf_t* conf_info);

static int is_fsize_exceeding(const struct conf_t* conf_info);

/**
 * Create a new thread to read file /proc/<pid>/stat,
 * so parent can get child's resource usage in real time,
 * kill child process when its resource usage exceed a
 * specified amount.
 */
static void* thread_reslimit(void* arg);

/**
 * Return child's exit status according to its resource usage
 */
int get_proc_result(const struct conf_t* conf_info,
                    const struct res_usage_t* res_usage);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __RESLIMIT_H__ */
