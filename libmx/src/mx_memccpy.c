#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    unsigned char *dst_ptr = dst;
    const unsigned char *src_ptr = src;
    unsigned char uc = (unsigned char)c;

    for (size_t i = 0; i < n; i++) {
        dst_ptr[i] = src_ptr[i];
        if (src_ptr[i] == uc) {
            return &dst_ptr[i + 1];
        }
    }

    return NULL;
}
