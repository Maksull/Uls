#include "libmx.h"

bool mx_is_start_substr(const char *str, const char* sub)
{
        while (*str && *sub && *str == *sub)
        {
            str++;
            sub++;
        }
        if (!*sub) 
        {
            return true; 
        }
        
        return false;
}

char *mx_strstr(const char *haystack, const char *needle) {
    while (*haystack) {
        if (mx_is_start_substr(haystack,needle)) 
        {
            return (char*)haystack; 
        }
        haystack++;
    }

    return NULL;
}
