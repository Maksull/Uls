#include "../inc/uls.h"

void mx_join(char **res, char *s2) {
    char *new_str = mx_strnew(mx_strlen(*res) + mx_strlen(s2));
    int i = 0;
    int a = -1;
    char *s1 = *res;

    while(s1[++a]) {
        new_str[i] = s1[a];
        i++;
    }
    a = -1;
    while(s2[++a]) {
        new_str[i] = s2[a];
        i++;
    }
    mx_strdel(&(*res));
    *res = new_str;
}
