//
// Created by 11135 on 2020/11/19.
//
#include "ourhdr.h"
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        err_quit("usage: a.out <pathname>");
    if (access(argv[1], F_OK) < 0)
        err_ret("access error for %s", argv[1]);
    else
        printf("read access OK\n");
    if (open(argv[1], O_RDONLY) < 0)
        err_ret("open error for %s", argv[1]);
    else
        printf("open for reading OK");
    exit(0);
}
