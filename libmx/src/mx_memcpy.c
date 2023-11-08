#include "libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    unsigned char *dst_ptr = dst;
    const unsigned char *src_ptr = src;

    for (size_t i = 0; i < n; i++) {
        dst_ptr[i] = src_ptr[i];
    }

    return dst;
}
