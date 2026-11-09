#include "ourhdr.h"

int report_errno(void)
{
    errno = EACCES;
    err_ret("cannot open %s", "input");
    return errno == EACCES ? 0 : 1;
}
