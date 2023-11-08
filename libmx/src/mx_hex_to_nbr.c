#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex)
{
    if (!hex || *hex == '\0')
    {
        return 0;
    }

    unsigned long result = 0;
    unsigned long base = 1;
    int len = mx_strlen(hex);

    for (int i = len - 1; i >= 0; i--)
    {
        char c = hex[i];
        unsigned long digit = 0;

        if (c >= '0' && c <= '9')
        {
            digit = c - '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            digit = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f')
        {
            digit = c - 'a' + 10;
        }
        else
        {
            return 0;
        }

        result += digit * base;
        base *= 16;
    }

    return result;
}
