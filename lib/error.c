#include "ourhdr.h"

char *pname = NULL;

static void report(int include_errno, const char *format, va_list arguments)
{
    int saved_errno = errno;
    fflush(stdout);
    flockfile(stderr);
    vfprintf(stderr, format, arguments);
    if (include_errno) fprintf(stderr, ": %s", strerror(saved_errno));
    fputc('\n', stderr);
    funlockfile(stderr);
    fflush(stderr);
    errno = saved_errno;
}

void err_ret(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    report(1, format, arguments);
    va_end(arguments);
}

void err_msg(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    report(0, format, arguments);
    va_end(arguments);
}

_Noreturn void err_sys(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    report(1, format, arguments);
    va_end(arguments);
    exit(EXIT_FAILURE);
}

_Noreturn void err_quit(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    report(0, format, arguments);
    va_end(arguments);
    exit(EXIT_FAILURE);
}

_Noreturn void err_dump(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    report(1, format, arguments);
    va_end(arguments);
    abort();
}
