#include "../inc/uls.h"

// Function to calculate the maximum width of various information fields for formatting
t_width mx_calculate_max_width(t_list *files_info, t_configuration *configuration) {
    t_width width = initialize_width();

    // Loop through file information to determine maximum widths
    while (files_info != NULL) {
        t_file_info *file_info = files_info->data;

        // Calculate and update maximum widths based on file information
        char *nlinks = mx_lltoa(file_info->stat.st_nlink);
        if (width.links < mx_strlen(nlinks))
        {
            width.links = mx_strlen(nlinks);
        }
            
        free(nlinks);
        if (width.user < mx_strlen(file_info->user))
        {
            width.user = mx_strlen(file_info->user);
        }
            
        if (width.group < mx_strlen(file_info->group))
        {
            width.group = mx_strlen(file_info->group);
        }

        int size_len = 5;
        if (S_ISCHR(file_info->stat.st_mode) || S_ISBLK(file_info->stat.st_mode)) {
            char *wsize = mx_nbr_to_hex(file_info->stat.st_rdev);
            size_len = mx_strlen(wsize) + 2;
            free(wsize);
        } else if (!configuration->display_human_readable_size) {
            char *wsize = mx_lltoa(file_info->stat.st_size);
            size_len = mx_strlen(wsize);
            free(wsize);
        }
        if (width.size < size_len)
        {
            width.size = size_len;
        }

        // Move to the next file information in the list
        files_info = files_info->next;
    }

    // Return the calculated maximum widths
    return width;
}
