#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    const unsigned char *s_ptr = (const unsigned char *)s;
    unsigned char uc = (unsigned char)c;

    if (n == 0) {
        return NULL;
    }

    for (size_t i = n; i > 0; i--) {
        if (s_ptr[i - 1] == uc) {
            return (void *)&s_ptr[i - 1];
        }
    }

    return NULL;
}
