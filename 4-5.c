//
// Created by 11135 on 2020/11/19.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ourhdr.h"

int main(int argc, char **argv)
{
    if (open("王八蛋.txt", O_RDWR) < 0)
        err_sys("open error");
    if (unlink("王八蛋.txt") < 0)
        err_sys("unlink error");
    printf("file unlinked\n");
    sleep(15);
    printf("done\n");

    exit(0);
}
