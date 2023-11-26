#include "../inc/uls.h"

// Function to print a string to the standard error output (file descriptor 2)
void mx_printerr(const char *s) {
    write(2, s, mx_strlen(s)); // Writes the given string to the standard error output
}
