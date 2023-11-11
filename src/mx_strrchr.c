#include "../inc/uls.h"

char *mx_strrchr(const char *s, int c)
{
    if (s == NULL) {
        return NULL;
    }

    int len = mx_strlen(s);

    for (int i = len; i >= 0; i--) {
        if (s[i] == c) {
            return (char *)&(s[i]);
        }
    }

    return NULL;
}
