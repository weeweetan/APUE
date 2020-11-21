//
// Created by 11135 on 2020/11/21.
//
#include "ourhdr.h"

int main(int argc, char **argv)
{
    char    *ptr;
    int     size;

    if (chdir("/usr/spool/uucppulic") < 0)
        err_sys("chdir failed");
    ptr = path_alloc(&size);
    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");
    printf("cwd = %s\n", ptr);
    exit(0);
}
