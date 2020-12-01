//
// Created by 11135 on 2020/11/30.
//
#include "ourhdr.h"
#include <pthread.h>

pthread_t ntid;

void printids(const char *s)
{
    pid_t       pid;
    pthread_t   tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);

}

void * thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void *)0);
}

int main(int argc, char **argv)
{
    int err;

    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
        err_quit(err, "can't create thread");
    printids("main thread:");
    sleep(1);
    exit(0);
}
