#include "../inc/uls.h"

// Function to print a character to the standard error
static void print_err_char(char flag) 
{
    write(2, &flag, 1);
}

// Function to print an error message for an invalid flag
void mx_print_flag_err(char flag) 
{
    // Print an error message for an invalid flag
    mx_printerr(ULS_STR);
    mx_printerr(ILOP_STR);
    print_err_char(flag);
    mx_printerr("\n");
    mx_printerr(USAGE_STR);
}

// Function to print an error message for a directory access issue
void mx_print_dir_error(const char* error, const char* dir_name) 
{
    // Print an error message indicating the directory and the nature of the error
    mx_printerr(ULS_STR);
    mx_printerr(dir_name);
    mx_printerr(": ");
    mx_printerr(error);
    mx_printerr("\n");
}

// Function to print an error message for a permission-related directory access issue
void mx_print_perm_dir_error(const char* error, const char* dir_path, bool is_single) 
{
    // Duplicate the directory path to avoid modifying the original
    char* path = mx_strdup(dir_path);
    
    // Extract the directory name from the path
    char* dir_name = mx_strrchr(path, '/');

    // Print the directory path (if not a single directory)
    if (!is_single) {
        mx_printstr(dir_path);
        mx_printstr(":\n");
    }

    // Print an error message for the permission-related directory access issue
    mx_printerr(ULS_STR);
    mx_printerr((dir_name != NULL) ? ++dir_name : dir_path);
    mx_printerr(": ");
    mx_printerr(error);
    mx_printerr("\n");

    // Free the duplicated path
    mx_strdel(&path);
}
