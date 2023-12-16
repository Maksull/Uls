#include "../inc/uls.h"

// Function to print a number aligned within a specified width
void mx_print_number_aligned(long long number, int width) {
    // Convert the number to a string and print it aligned within the specified width
    char *str_number = mx_lltoa(number);
    mx_print_aligned(str_number, width, true);
    free(str_number);
}
