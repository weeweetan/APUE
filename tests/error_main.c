#include "ourhdr.h"
#include "ourhdr.h" /* Repeated inclusion must not define any symbols. */

int report_errno(void);

int main(int argc, char **argv)
{
    if (argc != 2) return 2;
    if (strcmp(argv[1], "long") == 0) {
        char *message = malloc(32769);
        if (message == NULL) return 1;
        memset(message, 'x', 32768);
        message[32768] = '\0';
        errno = EBUSY;
        err_msg("begin:%s:end", message);
        free(message);
        return errno == EBUSY ? 0 : 1;
    }
    if (strcmp(argv[1], "return") == 0) return report_errno();
    if (strcmp(argv[1], "sys") == 0) {
        errno = ENOENT;
        err_sys("missing %s", "input");
    }
    if (strcmp(argv[1], "quit") == 0) err_quit("bad argument: %d", 42);
    return 2;
}
