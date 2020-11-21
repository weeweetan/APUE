//
// Created by 11135 on 2020/11/21.
//
#include <sys/types.h>
#include <sys/stat.h>
#include "ourhdr.h"

int main(int argc, char **argv)
{
    int         i;
    struct stat buf;
    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &buf) < 0) {
            err_ret("lstat error");
            continue;
        }
        printf("dev = %d/%d", major(buf.st_dev), minor(buf.st_dev));
        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
            printf(" (%s) rdev = %d/%d", (S_ISCHR(buf.st_mode)) ? "character" : "block", major(buf.st_dev), minor(buf.st_dev));
        }
        printf("\n");
    }
    exit(0);
}
