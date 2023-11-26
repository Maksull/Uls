#include "libmx.h"

char *mx_itoa(int number) {
    int len = 0;
    int n = number;
    char *str = NULL;

    if (number == 0) {
        return mx_strdup("0");
    }
    if (number == -2147483648) {
        return mx_strdup("-2147483648");
    }
    if (number < 0) {
        len++;
        n *= -1;
    }
    for (int i = n; i > 0; i /= 10) {
        len++;
    }
    str = mx_strnew(len);
    for (int i = 0; i < len; i++) {
        str[i] = n % 10 + 48;
        n /= 10;
    }
    if (number < 0) {
        str[len - 1] = '-';
    }
    mx_str_reverse(str);
    return str;
}
