//
// Created by 11135 on 2020/11/17.
//

#include <errno.h>
#include "ourhdr.h"

int main(int argc, char **argv)
{
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
    errno = ENOENT;
    perror(argv[0]);
    exit(0);
}