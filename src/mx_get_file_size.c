#include "../inc/uls.h"

static char* format_size_str(float d, int precision, char* letter) 
{
    char *file_size_str;
    float tmp = d;

    if ((int)tmp / 10 > 0) {
        tmp = mx_round(tmp, 0);
        file_size_str = mx_itoa(tmp);

    } else {
        tmp = mx_round(tmp, precision);
        file_size_str = mx_dtoa(tmp, precision);
    }

    mx_strcat(file_size_str, letter);

    return file_size_str;
}

char* mx_get_file_size(long int f_size) 
{
    int Kbyte = 1024;
    int Mbyte = 1024 * 1024;
    int Gbyte = 1024 * 1024 * 1024;
    long Tbyte = 1024 * 1024 * (long)1024 * 1024;
    long Pbyte = 1024 * 1024 * (long)1024 * 1024 * 1024;

    long int file_size = f_size;
    char *file_size_str;
    float tmp;

    if (file_size >= Pbyte) {
        tmp = (float)file_size / Pbyte;
        file_size_str = format_size_str(tmp, 1, "P");

    } else if (file_size >= Tbyte) {
        tmp = (float)file_size / Tbyte;
        file_size_str = format_size_str(tmp, 1, "T");

    } else if (file_size >= Gbyte) {
        tmp = (float)file_size / Gbyte;
        file_size_str = format_size_str(tmp, 1, "G");

    } else if (file_size >= Mbyte) {
        tmp = (float)file_size / Mbyte;
        file_size_str = format_size_str(tmp, 1, "M");

    } else if (file_size >= 1000) {
        tmp = (float)file_size / Kbyte;
        file_size_str = format_size_str(tmp, 1, "K");

    } else 
    {
        file_size_str = mx_itoa(file_size);
        mx_strcat(file_size_str, "B");
    }
    
    return mx_strdup(file_size_str);
}
