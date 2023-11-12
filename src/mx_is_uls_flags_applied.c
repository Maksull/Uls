#include "../inc/uls.h"

// Function to check if a file name represents a hidden file (starts with a dot)
static bool is_hidden_file(const char* name) 
{
    return (name[0] == '.');
}

// Function to check if hidden files should be skipped based on flags
static bool should_skip_hidden_files(t_uls_flags* flags, const char* file_name) 
{
    // Skip hidden files if the 'a' or 'A' flag is not present
    return (!flags->a && !flags->A && (is_hidden_file(file_name) || mx_is_implied_dir(file_name)));
}

// Function to check if implied directories should be skipped based on flags
static bool should_skip_implied_dir(t_uls_flags* flags, const char* file_name) 
{
    // Skip implied directories if the 'A' flag is present
    return (flags->A && mx_is_implied_dir(file_name));
}

// Function to check if uls flags are applied to a given file
bool mx_is_uls_flags_applied(t_uls_flags* flags, char* file_name) 
{
    // Skip files based on flags using helper functions
    if (should_skip_hidden_files(flags, file_name) || should_skip_implied_dir(flags, file_name)) 
    {
        return false;
    }

    // Return true if uls flags are applied to the file
    return true;
}
