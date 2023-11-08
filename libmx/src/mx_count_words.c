#include "libmx.h"

int mx_count_words(const char *str, char c) {
    if (str == NULL)
    {
        return -1;
    }
    int count = 0;
    bool check = false;
    while (*str) 
    {
        if (*str == c) 
        {
            check = false;
        } 
        else if (!check) 
        {
            check = true;
            count++;
        }
        str++;
    }
    return count;
}
