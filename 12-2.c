//
// Created by 11135 on 2020/12/9.
//
#include <limits.h>
#include <string.h>

#define MAXSTRINGSZ     4096
static char env_buf[MAXSTRINGSZ];
extern char **environ;

char *getenv(const char *name)
{
    int         i, len;
    len = strlen(name);
    for (i = 0; environ[i] != NULL; ++i) {
        if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '='))
        {
            strncpy(env_buf, &environ[i][len+1], MAXSTRINGSZ - 1);
            return (env_buf);
        }
    }
    return (NULL);
}
