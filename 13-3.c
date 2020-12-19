//
// Created by 11135 on 2020/12/19.
//
#include "ourhdr.h"
#include <pthread.h>
#include <syslog.h>
#include <13-1.c>

sigset_t mask;
extern int already_running(void);
extern void daemonize(const char *cmd);

void reread(void)
{

}

void *thr_fn(void *arg)
{
    int err, signo;

    for (;;)
    {
        sigwait(&mask, &signo);
        if (err != 0)
        {
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }

        switch (signo)
        {
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;
            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(0);
            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }

    }
    return (void *)0;
}

int main(int argc, char **argv)
{
    int             err;
    pthread_t       tid;
    char            *cmd;
    struct sigaction sa;

    if ((cmd = strchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    daemonize(cmd);

    if (already_running())
    {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        err_quit("%s: can't restore SIGHUP default");
    }

    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
        err_quit("SIG_BLOCK error");

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0)
        err_quit("can't create thread");


    exit(0);

}