//
// Created by 11135 on 2020/11/18.
//
#include "ourhdr.h"

#define BUFFSIZE 8192

int main(void)
{
    ssize_t n;
    char buf[BUFFSIZE];

    for (;;) {
        n = read(STDIN_FILENO, buf, sizeof buf);
        if (n == 0) break;
        if (n < 0) {
            if (errno == EINTR) continue;
            err_sys("read error");
        }
        size_t written = 0;
        while (written < (size_t)n) {
            ssize_t count = write(STDOUT_FILENO, buf + written, (size_t)n - written);
            if (count < 0) {
                if (errno == EINTR) continue;
                err_sys("write error");
            }
            if (count == 0) err_quit("write error: zero bytes written");
            written += (size_t)count;
        }
    }
    exit(0);
}
