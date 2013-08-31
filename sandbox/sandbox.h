#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct res_usage_t;

int start_sandbox(const char* conf_file, struct res_usage_t* res_usage);

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* __SANDBOX_H__ */
