#include "../inc/uls.h"

// Function to print a string to the standard error
void mx_printerr(const char *s)
{
    // Write the string to the standard error using the write system call
    write(2, s, mx_strlen(s));
}
