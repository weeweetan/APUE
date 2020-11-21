//
// Created by 11135 on 2020/11/18.
//
#include <sys/types.h>
#include "ourhdr.h"

int main(int argc, char **argv)
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("cannot seek\n");
    else
        printf("seek Ok\n");
    exit(0);
}
