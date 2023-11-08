#include "../inc/uls.h"

void mx_output_for_F(mode_t mode) 
{
    mode_t modified_mode = mode & S_IFMT;

    if (modified_mode == S_IFDIR)
    {
        mx_printchar('/');
    }
    else if (modified_mode == S_IFLNK)
    {
        mx_printchar('@');
    }
    else if (modified_mode == S_IFSOCK)
    {
        mx_printchar('=');
    }
    else if (modified_mode == S_IFIFO)
    {
        mx_printchar('|');
    }
    else if (IS_X(mode) && IS_R(mode))
    {
        mx_printchar('*');
    }
    else if (modified_mode == S_IFWHT)
    {
        mx_printchar('%');
    }
}
