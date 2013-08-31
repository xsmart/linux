/*****************************************************************************
 *        File: file_access.c
 * Description: Check what files child process opened for ptrace sandbox
 *      Author: Lei              
 *****************************************************************************/
#include <sys/ptrace.h>
#include <sys/user.h>      /* For struct user_regs_struct regs */
#include <fcntl.h>         /* For O_RDONLY etc */
#include <string.h>

#include "file_access.h"
#include "common.h"
#include "config.h"

// PTRACE_PEEKDATA reads a word from child's memory at once,
// and the size of a "word" is 32 bits(4 bytes) in 32-bit Linux
#define WORD 4

static int check_file_access(const struct conf_t* conf_info,
                             char* file_name, int open_flag)
{
    int i;
    // check file name
    for(i = 0; i < conf_info->file_num; i++)
    {
        if(strcmp(file_name, conf_info->file_list[i].file) == 0)
            break;
    }

    if(i == conf_info->file_num)
    {
        return RET_ERR;
    }

    int permit_flag = 0;
    char* p_access_flag = (char*)conf_info->file_list[i].access_flag;
    int has_read_flag = 0;
    int has_write_flag = 0;

    // check open flag
    while(*p_access_flag != '\0')
    {
        switch (*p_access_flag)
        {
            case 'r': 
                has_read_flag = 1;
                permit_flag |= O_RDONLY;
                break;
            case 'w': 
                has_write_flag = 1;
                permit_flag |= O_WRONLY;
                break;
            default:
                fprintf(stderr, "Warning,unkonwn open flag: %c\n", *p_access_flag);
                break;
        }
        p_access_flag ++;
    }

    if(has_read_flag && has_write_flag)
    {
        return RET_OK;
    }
    else
    {
        return (permit_flag | open_flag == permit_flag) ? RET_OK:RET_ERR;
    }
}

static int get_open_file(pid_t child_pid, const struct conf_t* conf_info,
                         char* file_name, int* open_flag)
{
    struct user_regs_struct regs;
    if(ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
    {
        fprintf(stderr, "Get SYS_open paraments error\n");
        return RET_ERR;
    }

    long file_name_addr = regs.ebx;

    union word
    {
        int value;
        char string[WORD];
    } buffer;

    int i = 0;
    for(i = 0; i < MAX_PATH; i += WORD)
    {
        buffer.value = ptrace(PTRACE_PEEKDATA, child_pid, file_name_addr + i, NULL);
        memcpy(file_name + i, buffer.string, WORD);
    }
    *open_flag = regs.ecx;
    return RET_OK;
}

int check_open_file(pid_t child_pid, const struct conf_t* conf_info)
{
    char file_name[MAX_PATH];
    int open_flag = 0;

    if(get_open_file(child_pid, conf_info, file_name, &open_flag) == RET_OK)
    {
        return check_file_access(conf_info, file_name, open_flag);
    }
    return RET_ERR;
}

