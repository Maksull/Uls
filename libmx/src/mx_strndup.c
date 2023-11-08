#include "libmx.h"

char *mx_strndup(const char *s1, size_t n) 
{
    size_t len = mx_strlen(s1);
    if (n < len)
    {
        len = n;
    }
    char *newstr = mx_strnew(len);
    mx_strncpy(newstr, s1, len);
    
    return newstr;
}
