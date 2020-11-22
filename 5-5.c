//
// Created by 11135 on 2020/11/21.
//
#include "ourhdr.h"
#include <errno.h>
#include <sys/stat.h>

void make_temp(char *template);

int main(int argc, char **argv)
{
    char good_template[] = "/tmp/dirXXXXXX";
    char *bad_template = "/tmp/dirXXXXXX";

    printf("trying to create first temp file...\n");
    make_temp(good_template);
    printf("trying to create second temp file...\n");
    make_temp(bad_template);
    exit(0);
}

void make_temp(char *template)
{
    int         fd;
    struct stat sbuf;

    if ((fd = mkstemp(template)) < 0)
        err_sys("can't create temp file");
    printf("temp name = %s\n", template);
    close(fd);
    if (stat(template, &sbuf) < 0)
    {
        if (errno == ENOENT)
            printf("file doesn't exist\n");
        else
            err_sys("file doesn't exists\n");

    } else {
        printf("file exists\n");
        unlink(template);
    }
}
