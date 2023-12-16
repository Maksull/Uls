#include "../inc/uls.h"

static void calculate_field_width(int *max_width, const char *field_value) {
    int value_length = mx_strlen(field_value);
    if (*max_width < value_length) {
        *max_width = value_length;
    }
}

static int calculate_size_length(t_file_info *file_info, t_configuration *configuration) {
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

    return size_len;
}

static void calculate_and_update_widths(t_width *width, t_file_info *file_info, t_configuration *configuration) {
    char *nlinks = mx_lltoa(file_info->stat.st_nlink);
    calculate_field_width(&width->links, nlinks);
    free(nlinks);
    calculate_field_width(&width->user, file_info->user);
    calculate_field_width(&width->group, file_info->group);

    int size_len = calculate_size_length(file_info, configuration);
    calculate_field_width(&width->size, mx_itoa(size_len));
}

static t_width initialize_width() {
    t_width width;
    width.links = 0;
    width.user = 0;
    width.group = 0;
    width.size = 0;
    return width;
}

// Function to calculate the maximum width of various information fields for formatting
static t_width calculate_max_width(t_list *files_info, t_configuration *configuration) {
    t_width width = initialize_width();

    while (files_info != NULL) {
        t_file_info *file_info = files_info->data;
        calculate_and_update_widths(&width, file_info, configuration);
        files_info = files_info->next;
    }

    return width;
}

// Function to print detailed information for multiple files
void mx_print_with_info(t_list *files_info, t_configuration *configuration) {
    // Calculate the maximum width of various fields for formatting
    t_width max_width = calculate_max_width(files_info, configuration);

    // Loop through file information and print detailed information for each file
    while (files_info != NULL) {
        mx_print_file_info_detailed(files_info->data, &max_width, configuration);
        files_info = files_info->next;
    }
}
