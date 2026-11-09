//
// Created by 11135 on 2020/11/17.
//
#include "ourhdr.h"

int main(void)
{
    printf("uid = %lu, gid = %lu\n", (unsigned long)getuid(), (unsigned long)getgid());
    exit(0);
}
