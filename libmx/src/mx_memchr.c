#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    const unsigned char *s_ptr = s;
    unsigned char uc = (unsigned char)c;

    for (size_t i = 0; i < n; i++) {
        if (s_ptr[i] == uc) {
            return (void *)&s_ptr[i];
        }
    }

    return NULL;
}
