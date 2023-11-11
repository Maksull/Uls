#include "../inc/uls.h"

static void insert_decimal_point(char* d_str, int precision, int len) {
    for (int i = len + 1; i >= len - precision; i--) {
        if (i == len - precision) {
            d_str[i] = '.';
            break;
        } else {
            d_str[i] = d_str[i - 1];
        }
    }
}

char *mx_dtoa(float d, int precision) {
    float tmp = d;
    
    // Multiply by 10^precision
    for (int i = 0; i < precision; i++) {
        tmp *= 10;
    }

    // Convert to string
    char *d_str = mx_itoa((int)tmp);
    int len = mx_strlen(d_str);

    // Reallocate memory to accommodate the decimal point
    d_str = mx_realloc(d_str, len + precision + 2);

    // Insert the decimal point
    insert_decimal_point(d_str, precision, len);

    return d_str;
}
