//
// Created by 11135 on 2020/11/23.
//
#include "ourhdr.h"

static void charatatime(char *);

int main(int argc, char **argv)
{
    pid_t       pid;

    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        charatatime("output from child\n");
    }
    else
        charatatime("output from parent\n");

    exit(0);
}


static void charatatime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++);) {
        putc(c, stdout);
    }
}
