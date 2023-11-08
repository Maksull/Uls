#include "libmx.h"

char *mx_strcpy(char *dst, const char *src) 
{
    char *original_dst = dst;
    
    while (*src) {
        *dst++ = *src++;
    }
    
    *dst = '\0';
    
    return original_dst;
}
