//
// Created by 11135 on 2020/11/17.
//
#include "ourhdr.h"

int main(int argc, char **argv)
{
    int c;

    while ((c = getc(stdin)) != EOF)
        if (putc(c, stdout) == EOF)
            err_sys("output error");

    if (ferror(stdin))
        err_sys("input error");
    exit(0);
}
