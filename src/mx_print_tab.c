#include "../inc/uls.h"

void mx_print_tab(int length, int max_length) {
    int counter = 0;
    int p = max_length - length;

    if (p % 8 != 0) {
        counter = (p / 8) + 1;
    } else {
        counter = p / 8;
    }
    
    for (int a = 0; a < counter; a++) {
        mx_printchar('\t');
    }
}
