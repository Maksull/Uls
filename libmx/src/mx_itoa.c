#include "libmx.h"

char *mx_itoa(int number) {
    if (number == 0)
    {
        return "0";
    }
    else if (number == -2147483648) 
    {
        return "-2147483648";
    }
    int temp = number;
    int length = 0;
    while (temp)
    {
        temp /=10;
        length++;
    }
    char *str = malloc(length);
    if (number < 0)
    {
        str[0] = '-';
        number = -number;
        for (int i = length; i > 0; i--)
        {
            str[i] = (number % 10) + 48 + '\0';
            number /= 10;
        }
    }
    else
    {
        for (int i = length - 1 ; i >= 0; i--)
        {
            str[i] = (number % 10) + 48 + '\0';
            number /= 10;
        }
    }
    return str;
}
