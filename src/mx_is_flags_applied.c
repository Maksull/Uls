#include "../inc/uls.h"

static bool is_hidden_file(const char* name) 
{
    return (name[0] == '.');
}

bool mx_is_uls_flags_applied(t_uls_flags* flags, char* file_name) 
{
    if (!flags->a && !flags->A && (is_hidden_file(file_name) || mx_is_implied_dir(file_name)))
    {
        return false;
    }
    if (flags->A && (mx_is_implied_dir(file_name)))
    {
        return false;
    }

    return true;
}
