#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) {
        return -1; 
    }

    int sub_len = mx_strlen(sub);

    if (sub_len == 0) {
        return 0;
    }

    int count = 0;
    char *ptr = (char *)str;

    while ((ptr = mx_strstr(ptr, sub)) != NULL) {
        count++;
        ptr += sub_len;
    }

    return count;
}
