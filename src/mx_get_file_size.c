#include "../inc/uls.h"

char* mx_get_file_size(long int f_size) 
{
    static const char* size_suffix[] = {"B", "K", "M", "G", "T", "P"};
    static const int size_suffix_count = sizeof(size_suffix) / sizeof(size_suffix[0]);
    
    long int file_size = f_size;
    int index = 0;

    while (file_size >= 1024 && index < size_suffix_count - 1) {
        file_size /= 1024;
        index++;
    }

    char* file_size_str;

    if (index == 0 || file_size % 10 == 0) {
        file_size_str = mx_itoa(file_size);
    } else {
        float tmp = (float)file_size;
        file_size_str = mx_dtoa(tmp, 1);
    }

    mx_strcat(file_size_str, size_suffix[index]);

    return mx_strdup(file_size_str);
}
