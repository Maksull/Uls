#include "../inc/uls.h"

// Function to print the name of a directory followed by a colon and a newline
void mx_print_dir_name(const char* dir_name) 
{
    // Print the directory name
    mx_printstr(dir_name);

    // Print a colon to separate the directory name from the contents
    mx_printstr(":");

    // Print a newline for better formatting
    mx_printstr("\n");
}
