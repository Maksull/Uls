#include "../inc/uls.h"

// Function to insert a decimal point into the string representation of a float
static void insert_decimal_point(char* d_str, int precision, int len) {
    // Shift characters to make room for the decimal point
    for (int i = len + 1; i >= len - precision; i--) {
        if (i == len - precision) {
            // Insert the decimal point at the appropriate position
            d_str[i] = '.';
            break;
        } else {
            // Shift characters to the right
            d_str[i] = d_str[i - 1];
        }
    }
}

// Function to convert a float to a string representation with a specified precision
char *mx_dtoa(float d, int precision) {
    float tmp = d;
    
    // Multiply by 10^precision to preserve decimal places during conversion
    for (int i = 0; i < precision; i++) {
        tmp *= 10;
    }

    // Convert the multiplied float to a string
    char *d_str = mx_itoa((int)tmp);
    int len = mx_strlen(d_str);

    // Reallocate memory to accommodate the decimal point
    d_str = mx_realloc(d_str, len + precision + 2);

    // Insert the decimal point into the string representation
    insert_decimal_point(d_str, precision, len);

    return d_str;
}
