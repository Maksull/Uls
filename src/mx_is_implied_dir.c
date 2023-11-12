#include "../inc/uls.h"

// Function to check if a directory name represents an implied directory ('.' or '..')
bool mx_is_implied_dir(const char* dir_name) 
{
    // Return true if the directory name is '.' or '..'
    return (!mx_strcmp(dir_name, ".") || !mx_strcmp(dir_name, ".."));
}
