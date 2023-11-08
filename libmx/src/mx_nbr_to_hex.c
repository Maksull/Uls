#include "libmx.h"

char* mx_nbr_to_hex(unsigned long nbr) {
    unsigned long temp = nbr;
    int length = 0;
    
    while (temp > 0) {
        temp /= 16;
        length++;
    }

    char* hex_str = (char*)malloc((length + 1) * sizeof(char));
    if (hex_str == NULL) {
        return NULL;
    }

    for (int i = length - 1; i >= 0; i--) {
        int remainder = nbr % 16;
        if (remainder < 10) {
            hex_str[i] = '0' + remainder;
        } else {
            hex_str[i] = 'a' + (remainder - 10);
        }
        nbr /= 16;
    }

    hex_str[length] = '\0';

    return hex_str;
}
