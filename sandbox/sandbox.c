#include <stdio.h>
#include <string.h>

#include "sandbox.h"
#include "config.h"
#include "common.h"
#include "helper.h"
#include "ptrace.h"

#define APPARMOR "apparmor"
#define PTRACE   "ptrace"

int start_sandbox(const char* conf_file, struct res_usage_t* res_usage)
{
    struct conf_t conf_info;
    read_conf(conf_file, &conf_info); 

    if(strcmp(APPARMOR, conf_info.sandbox_opt) == 0)
    {
        return start_apparmor_helper(&conf_info, res_usage);
    }
    else if(strcmp(PTRACE, conf_info.sandbox_opt) == 0)
    {
        return start_ptrace_sandbox(&conf_info, res_usage);
    }

    fprintf(stderr, "unknown sandbox option: %s\n", conf_info.sandbox_opt);
    return RESULT_IE;
}
