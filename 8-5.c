//
// Created by 11135 on 2020/11/22.
//
#include "ourhdr.h"
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid;
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
    {
        if ((pid = fork()) < 0)
            err_sys("fork error");
        else if (pid > 0)
            exit(0);
        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getpid());
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");
    exit(0);

}
