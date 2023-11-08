#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (str == NULL || sub == NULL || replace == NULL) {
        return NULL;
    }

    int str_len = mx_strlen(str);
    int sub_len = mx_strlen(sub);
    int replace_len = mx_strlen(replace);

    int count = 0;
    const char *ptr = str;

    while ((ptr = mx_strstr(ptr, sub)) != NULL) {
        count++;
        ptr += sub_len;
    }

    if (count == 0) {
        return mx_strdup(str);
    }

    int result_len = str_len + (replace_len - sub_len) * count;
    char *result = (char *)malloc(result_len + 1);
    if (result == NULL) {
        return NULL;
    }

    char *dest = result;
    const char *src = str;

    while (*src) {
        if (mx_strstr(src, sub) == src) {
            mx_strcpy(dest, replace);
            dest += replace_len;
            src += sub_len;
        } else {
            *dest = *src;
            dest++;
            src++;
        }
    }

    *dest = '\0';

    return result;
}
