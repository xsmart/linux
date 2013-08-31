#include <string.h>
#include "config.h"

// keys for configuration file
// PS: use key-value in configuration file
// e.g. file  /home/username/config_file
#define KEY_FILE        "file"
#define KEY_CPU         "cpu"
#define KEY_MEM         "mem"
#define KEY_FSIZE       "fsize"
#define KEY_FILE_IN     "in"
#define KEY_FILE_OUT    "out"
#define KEY_FILE_ERR    "err"
#define KEY_FILE_LIST   "flist"
#define KEY_SANDBOX_OPT "sandbox"

#define MAX_KEY_LENGTH   256

int read_conf(const char* file_name,struct conf_t* conf_info)
{
   FILE* fd;
   fd = fopen(file_name , "r");

   if(NULL == fd)
   {
        fprintf(stderr,"Failed to open file: %s",file_name);
        return -1;
   }

   char key[MAX_KEY_LENGTH];

   //maybe there is no file list in configuration file
   conf_info->file_num = 0;
    
    while(!feof(fd))
    {
       fscanf(fd,"%s",key);
       
       if(strcmp(key,KEY_FILE) == 0)
       {
            fscanf(fd,"%s",conf_info->file_name);
       }
       else if(strcmp(key,KEY_CPU) == 0)
       {
            fscanf(fd,"%d",&conf_info->cputime); 
       }
       else if(strcmp(key,KEY_MEM) == 0)
       {
            fscanf(fd,"%d",&conf_info->memory);
       }
       else if(strcmp(key,KEY_FSIZE) == 0)
       {
            fscanf(fd,"%d",&conf_info->file_size);
       }
       else if(strcmp(key,KEY_FILE_IN) == 0)
       {
            fscanf(fd,"%s",conf_info->file_in);
       }
       else if(strcmp(key,KEY_FILE_OUT) == 0)
       {
            fscanf(fd,"%s",conf_info->file_out);
       }
       else if(strcmp(key,KEY_FILE_ERR) == 0)
       {
            fscanf(fd,"%s",conf_info->file_err);
       }
       else if(strcmp(key,KEY_SANDBOX_OPT) == 0)
       {
            fscanf(fd,"%s",conf_info->sandbox_opt);
       }
       else if(strcmp(key,KEY_FILE_LIST) == 0)
       {
            // read files that process can access
            read_file_list(fd,conf_info);
       }
       else
       {
            fprintf(stderr,"key: %s\n",key);
            fprintf(stderr,"Syntax error in configuration file.\n");
            return 1;
       }
    }
    fclose(fd);
    return 0;
}

void read_file_list(FILE* fd,struct conf_t* conf_info)
{
    conf_info->file_num=0;

    while(fscanf(fd,"%s",conf_info->file_list[conf_info->file_num].file) != EOF
        &&fscanf(fd,"%s",conf_info->file_list[conf_info->file_num].access_flag) != EOF)
    {
        conf_info->file_num++;
    }
}
