//
// Created by 11135 on 2020/12/10.
//
#include "ourhdr.h"
#include <pthread.h>

int         quitflag;
sigset_t    mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitlock = PTHREAD_COND_INITIALIZER;

void *thr_fn(void *arg)
{
    int     err, signo;
    for (;;)
    {
        err = sigwait(&mask, &signo);
        if (err != 0)
            err_quit("sigwait failed");
        switch (signo)
        {
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&lock);
                quitflag = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&waitlock);
                return(0);
            default:
                printf("unexpected signal %d\n", signo);
                exit(1);
        }
    }
}


int main(int argc, char **argv)
{
    int         err;
    sigset_t    oldmask;
    pthread_t   tid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);

    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0)
        err_quit("SIG_BLOCK error");

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0)
        err_quit("can't create thread");

    pthread_mutex_lock(&lock);
    while (quitflag == 0)
        pthread_cond_wait(&waitlock, &lock);
    pthread_mutex_unlock(&lock);

    quitflag = 0;
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_quit("SIG_SETMASK error");
    exit(0);
}
