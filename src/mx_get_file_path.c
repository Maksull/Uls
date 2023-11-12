#include "../inc/uls.h"

// Function to concatenate directory name and file name to form a complete file path
char* mx_get_file_path(const char* dir_name, const char* file_name) 
{
    // Allocate memory for the complete file path
    char* file_path = mx_strnew(mx_strlen(dir_name) + mx_strlen(file_name) + 1);

    // Copy the directory name to the file path
    mx_strcpy(file_path, dir_name);

    // Concatenate a forward slash to separate directory and file names
    mx_strcat(file_path, "/");

    // Concatenate the file name to the file path
    mx_strcat(file_path, file_name);

    return file_path;
}
