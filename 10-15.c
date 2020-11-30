//
// Created by 11135 on 2020/11/30.
//

#include <signal.h>
#include "ourhdr.h"

static void sig_int(int);

void pr_mask(const char *str)
{
    sigset_t        sigset;
    int             errno_save;

    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0)
        err_sys("sigprocmask error");
    printf("%s", str);
    if (sigismember(&sigset, SIGINT))       printf("SIGINT ");
    if (sigismember(&sigset, SIGQUIT))       printf("SIGQUIT ");
    if (sigismember(&sigset, SIGUSR1))       printf("SIGUSR1 ");
    if (sigismember(&sigset, SIGALRM))       printf("SIGALRM ");

    printf("\n");
    errno = errno_save;
}

int main(int argc, char **argv)
{
    sigset_t newmask, oldmask, zeromask;
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
    pr_mask("in critical region: ");

    if (sigsuspend(&zeromask) != -1)
        err_sys("sigsuspend error");
    pr_mask("after return from sigsuspend: ");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    exit(0);
}

static void sig_int(int signo)
{
    pr_mask("\n sig_int: ");
    return;
}