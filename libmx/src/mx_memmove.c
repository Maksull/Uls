#include "libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    char *d = (char *)dst;
    const char *s = (const char *)src;

    if (d < s) {
        while (len--) {
            *d++ = *s++;
        }
    } else if (d > s) {
        d += len;
        s += len;
        while (len--) {
            *(--d) = *(--s);
        }
    }

    return dst;
}
