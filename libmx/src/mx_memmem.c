#include "libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len)
{
    if (big == NULL || little == NULL || big_len < little_len) {
        return NULL;
    }

    const char *h = (const char *)big;
    const char *n = (const char *)little;

    for (size_t i = 0; i <= big_len - little_len; i++) {
        if (h[i] == n[0] && mx_memcmp(&h[i], n, little_len) == 0) {
            return (void *)&h[i];
        }
    }

    return NULL;
}
