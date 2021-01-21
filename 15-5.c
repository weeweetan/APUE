//
// Created by 11135 on 2021/1/18.
//
#include "apue.h"
#include <ctype.h>

int main(int argc, char **argv)
{
    int c;

    while ((c = getchar()) != EOF)
    {
        if (isupper(c))
            c = tolower(c);
        if (putchar(c) == EOF)
            err_sys("output error");
        if (c == '\n')
            fflush(stdout);
    }
    exit(0);
}
