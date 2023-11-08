#include "../inc/uls.h"

bool mx_is_implied_dir(const char* dir_name) 
{
    return (!mx_strcmp(dir_name, ".") || !mx_strcmp(dir_name, ".."));
}
