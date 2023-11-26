#include "../inc/uls.h"

// Concatenates two strings and updates the pointer to the resulting string
void mx_join(char **res, char *s2) {
    // Allocating memory for the new string by calculating its total length
    char *new_str = mx_strnew(mx_strlen(*res) + mx_strlen(s2));
    int i = 0;
    int a = -1;
    char *s1 = *res; // Pointer to the first string

    // Copying characters from the first string to the new string
    while (s1[++a]) {
        new_str[i] = s1[a];
        i++;
    }

    a = -1;
    // Copying characters from the second string to the new string after the first string
    while (s2[++a]) {
        new_str[i] = s2[a];
        i++;
    }

    // Freeing memory occupied by the original string pointed to by 'res'
    mx_strdel(&(*res));
    // Updating the pointer 'res' to the new concatenated string
    *res = new_str;
}
