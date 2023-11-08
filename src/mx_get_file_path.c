#include "../inc/uls.h"

char* mx_get_file_path(const char* dir_name, const char* file_name) 
{
    char* file_path = mx_strnew(mx_strlen(dir_name) + mx_strlen(file_name) + 1);
    mx_strcpy(file_path, dir_name);
    mx_strcat(file_path, "/");
    mx_strcat(file_path, file_name);

    return file_path;
}
