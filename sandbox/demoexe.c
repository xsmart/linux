#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <memory.h>

double g_array[1000];
//double g_array2[1000000];

int main()
{
    memset(g_array, 1, sizeof(g_array));
    //memset(g_array2, 2, sizeof(g_array2)/sizeof(double));
    //printf("Here is demoexe...\n"); /* Test output file size */
    //printf("Press any key to stop\n");
    //char ch=getchar();
    //printf("ch you input: %c\n",ch);

    //while(1); /* Test CPU */
    int i;
    /*for(i = 0; i < 10000; i++)
    {
        printf("1234567890\n");
    }*/

    
    fprintf(stderr, "Hehe\n");
    /*int i=0;
    for(i=0;i<1000000;i++);*/
    //double array[10000];
    double array[5000]; /* Test memory */
    /*double* p_array = malloc(sizeof(double)*1000000);
    if(p_array == NULL)
    {
        fprintf(stderr, "Malloc error\n");
    }
    memset(p_array, 1, sizeof(double)*1000000);
    free(p_array);*/

    /* Test access to file system */
    char* file = "/home/lei/sandbox/file.data";
    int fd = open(file, O_WRONLY|O_TRUNC);
    //int fd = open(file, O_RDONLY);
    if(fd == -1)
    {
        fprintf(stderr,"Failed to open file: %s\n", file);
        //return -1;
    }
    char buffer[256];
    read(fd, buffer, 10);
    write(fd,"Hello,data file",15);
    write(fd,"Hello,data file",15);
    close(fd);  /* test file system */

    /* Test signal */
    /*if(kill(7692,SIGKILL) == -1)
        fprintf(stderr,"Failed to signal");*/

    /* Test system call */
    /*pid_t childpid;
    childpid=fork();

    if(childpid == -1)
    {
        fprintf(stderr,"Failed to fork");
        return 1;
    }

    if(childpid == 0)
    {
        fprintf(stderr,"Here is chile process");
        return 0;
    }
    else if(wait(NULL) != childpid)
    {
        fprintf(stderr,"Failed to wait for child process.");
    }
    else
    {
        printf("Here is parent process");
    } */  /* Test system call */
    printf("Test end\n");
    return 0;
}
