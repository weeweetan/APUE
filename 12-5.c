//
// Created by 11135 on 2020/12/11.
//
#include "ourhdr.h"
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void prepare(void)
{
    int         err;

    printf("preparing locks...\n");
    if ((err = pthread_mutex_lock(&lock1)) != 0)
        err_quit("can't lock lock1 in prepare handler");
    if ((err = pthread_mutex_lock(&lock2)) != 0)
        err_quit("can't lock lock2 in prepare handler");
}


void parent(void)
{
    int         err;

    printf("parent unlocking locks...\n");
    if ((err = pthread_mutex_lock(&lock1)) != 0)
        err_quit("can't lock lock1 in prepare handler");
    if ((err = pthread_mutex_lock(&lock2)) != 0)
        err_quit("can't lock lock2 in prepare handler");

}


void child(void)
{
    int         err;

    printf("child unlocking locks...\n");
    if ((err = pthread_mutex_lock(&lock1)) != 0)
        err_quit("can't lock lock1 in prepare handler");
    if ((err = pthread_mutex_lock(&lock2)) != 0)
        err_quit("can't lock lock2 in prepare handler");

}


void *thr_fn(void *arg)
{
    printf("thread started...\n");
    pause();
    return(0);
}

int main(int argc, char **argv)
{
    int         err;
    pid_t       pid;
    pthread_t   tid;

    if ((err = pthread_atfork(prepare, parent, child)) != 0)
        err_quit("can't install fork handlers");
    if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
        err_quit("can't create thread");
    sleep(2);
    printf("parent about to fork...\n");

    if ((pid = fork()) < 0)
        err_quit("fork failed");
    else if (pid == 0)
        printf("child returned form fork\n");
    else
        printf("parent returned from fork\n");
    exit(0);
}