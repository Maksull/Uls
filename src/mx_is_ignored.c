#include "../inc/uls.h"

// Function to check if a name should be ignored based on ignore_type
bool mx_is_ignored(const char *name, t_ignore_type ignore_type) {
    // Check if the name should be ignored based on the ignore_type
    if (ignore_type == HIDDEN && name[0] == '.')
    {
       return true; 
    }
    if (ignore_type == DOTS && (mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0))
    {
        return true;
    }
    
    return false;
}
