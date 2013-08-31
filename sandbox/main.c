#include <stdio.h>
#include <stdlib.h>

#include "sandbox.h"
#include "common/common.h"

const char* result_name[]=
{
    "Okay",
    "Time Limit Exceed",
    "Memory Limit Exceed",
    "Output Limit Exceed",
    "Internal Error"
};

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "main: parameter error\n");
        return EXIT_FAILURE;
    }

    enum result_t result;
    struct res_usage_t res_usage;
    result = start_sandbox(argv[1], &res_usage);

    fprintf(stderr, "result: %s\n",result_name[result]);
    if(result == RESULT_IE)
        return EXIT_FAILURE;
    else
    {
        fprintf(stderr, "CPU time: %ld MS\n", res_usage.elapsed_time);
        fprintf(stderr, "Memory : %ld KB\n", res_usage.max_rss);
        return EXIT_SUCCESS;
    }
}
