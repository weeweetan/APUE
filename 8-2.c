//
// Created by 11135 on 2020/11/22.
//

#include "ourhdr.h"

int globval = 6;
char buf[] = "a write to stdout\n";

int main(int argc, char **argv)
{
    int var;
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
        err_sys("write error");
    printf("before vfork\n");

    if ((pid = vfork()) < 0)
        err_sys("vfork error\n");
    else if (pid == 0)
    {
        var++;
        globval++;
        _exit(0);
    }

    printf("pid = %ld, glob = %d, val = %d\n", (long)getpid(), globval, var);
    exit(0);
}