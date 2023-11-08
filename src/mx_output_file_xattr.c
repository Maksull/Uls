#include "../inc/uls.h"

void mx_output_file_xattr(const char* path, bool is_h_on) 
{
    char attr_name[XATTR_LEN];
    char value[XATTR_LEN];
    int list_len = listxattr(path, attr_name, XATTR_LEN, XATTR_NOFOLLOW);
    int value_len = 0;

    for (int idx = 0; idx < list_len;) {
        mx_printstr("\t");
        mx_printstr(&attr_name[idx]);
        value_len = getxattr(path, &attr_name[idx], value, XATTR_LEN, 0, 0);

        mx_printstr("\t   ");
        if (value_len != -1)
        {
            mx_printint(value_len);
        }
        else {
            mx_printint(0);
        }

        is_h_on ? mx_printchar('B') : (void) 0;
        mx_printstr(" \n");
        idx += mx_strlen(&attr_name[idx]) + 1;
    }
}
