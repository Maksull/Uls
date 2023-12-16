#include "../inc/uls.h"

// Function to free allocated memory for a single file_info structure
void mx_free_file_info(t_file_info *file_info) {
    // Free allocated memory for various fields in file_info structure
    free(file_info->path);
    free(file_info->name);
    mx_strdel(&file_info->user);
    mx_strdel(&file_info->link);
    mx_strdel(&file_info->group);
    mx_del_strarr(&file_info->xattr_keys);
    acl_free(file_info->acl);
    free(file_info);
}
