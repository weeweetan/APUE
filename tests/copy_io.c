#include "ourhdr.h"

/* Keep real pipe I/O, but reproduce permitted partial/interrupted results. */
static unsigned int read_calls, write_calls;

static int selected(const char *mode)
{
    const char *value = getenv("APUE_COPY_MODE");
    return value != NULL && strcmp(value, mode) == 0;
}

static ssize_t copy_read(int descriptor, void *buffer, size_t count)
{
    ++read_calls;
    if ((selected("read-eintr") && read_calls == 1) ||
        (selected("mixed") && read_calls % 4 == 1)) {
        errno = EINTR;
        return -1;
    }
    if (selected("read-error") ||
        (selected("read-error-after-data") && read_calls > 1)) {
        errno = EIO;
        return -1;
    }
    if (selected("read-error-after-data") && count > 11) count = 11;
    if (selected("short-read") && count > 17) count = 17;
    if (selected("mixed") && count > 97) count = 97;
    return read(descriptor, buffer, count);
}

static ssize_t copy_write(int descriptor, const void *buffer, size_t count)
{
    ++write_calls;
    if ((selected("write-eintr") && write_calls == 1) ||
        (selected("mixed") && write_calls % 4 == 2)) {
        errno = EINTR;
        return -1;
    }
    if (selected("write-error") ||
        (selected("write-error-after-data") && write_calls > 1)) {
        errno = ENOSPC;
        return -1;
    }
    if (selected("write-zero") ||
        (selected("write-zero-after-data") && write_calls > 1)) return 0;
    if ((selected("short-write") || selected("mixed") ||
         selected("write-error-after-data") || selected("write-zero-after-data")) && count > 7)
        count = 7;
    return write(descriptor, buffer, count);
}

/* Include the actual chapter main after system prototypes, avoiding libc aliases. */
#define read copy_read
#define write copy_write
#include APUE_COPY_SOURCE
