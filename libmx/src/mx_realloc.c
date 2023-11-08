#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    if (ptr == NULL) {
        return malloc(size);
    }

    size_t old_size = *((size_t*)((char*)ptr - sizeof(size_t)));

    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }

    if (old_size > size) {
        old_size = size;
    }

    mx_memcpy(new_ptr, ptr, old_size);
    free(ptr);

    return new_ptr;
}
