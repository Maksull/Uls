#include "../inc/uls.h"

void mx_output_for_F(mode_t mode) 
{
    mode_t modified_mode = mode & S_IFMT;

    switch (modified_mode) {
        case S_IFDIR:
            mx_printchar('/');
            break;
        case S_IFLNK:
            mx_printchar('@');
            break;
        case S_IFSOCK:
            mx_printchar('=');
            break;
        case S_IFIFO:
            mx_printchar('|');
            break;
        case S_IFWHT:
            mx_printchar('%');
            break;
        default:
            if (IS_X(mode) && IS_R(mode)) {
                mx_printchar('*');
            }
            break;
    }
}
