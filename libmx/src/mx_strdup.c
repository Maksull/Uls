#include "libmx.h"

char *mx_strdup(const char *s1)
{
    if (s1 == NULL) {
        return NULL;
    }

    int len = mx_strlen(s1);

    char *dup_str = mx_strnew(len);
    mx_strcpy(dup_str, s1);

    return mx_strcpy(dup_str, s1);
}
