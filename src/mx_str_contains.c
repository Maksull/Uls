#include "../inc/uls.h"

bool mx_str_contains(char *str, char c) {
    if (str == NULL) {
        return false;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            return true;
        }
    }

    return false;
}
