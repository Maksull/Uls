#include "../inc/uls.h"

// Function to check if a name should be ignored because it's hidden
bool should_ignore_hidden(const char *name, t_ignore_type ignore_type) {
    return (ignore_type == HIDDEN && name[0] == '.');
}

// Function to check if a name should be ignored because it's dots
bool should_ignore_dots(const char *name, t_ignore_type ignore_type) {
    return (ignore_type == DOTS && (mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0));
}

// Function to check if a name should be ignored based on ignore_type
bool mx_is_ignored(const char *name, t_ignore_type ignore_type) {
    // Check if the name should be ignored based on the ignore_type
    if (should_ignore_hidden(name, ignore_type) || should_ignore_dots(name, ignore_type))
    {
       return true; 
    }
    
    return false;
}
