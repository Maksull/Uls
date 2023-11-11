#include "../inc/uls.h"

static bool is_hidden_file(const char* name) 
{
    return (name[0] == '.');
}

static bool should_skip_hidden_files(t_uls_flags* flags, const char* file_name) 
{
    return (!flags->a && !flags->A && (is_hidden_file(file_name) || mx_is_implied_dir(file_name)));
}

static bool should_skip_implied_dir(t_uls_flags* flags, const char* file_name) 
{
    return (flags->A && mx_is_implied_dir(file_name));
}

bool mx_is_uls_flags_applied(t_uls_flags* flags, char* file_name) 
{
    if (should_skip_hidden_files(flags, file_name) || should_skip_implied_dir(flags, file_name)) 
    {
        return false;
    }

    return true;
}
