#include "../inc/uls.h"

// Function to calculate the width of a field and update the maximum width if necessary
static void calculate_field_width(int *max_width, const char *field_value) {
    int value_length = mx_strlen(field_value);
    if (*max_width < value_length) {
        *max_width = value_length;
    }
}

// Function to calculate the length of the size field based on file type and configuration
static int calculate_size_length(t_file_info *file_info, t_configuration *configuration) {
    int size_len = 5; // Default size length

    if (S_ISCHR(file_info->stat.st_mode) || S_ISBLK(file_info->stat.st_mode)) {
        char *wsize = mx_nbr_to_hex(file_info->stat.st_rdev);
        size_len = mx_strlen(wsize) + 2; // Calculate size length for character/block devices
        free(wsize);
    } else if (!configuration->display_human_readable_size) {
        char *wsize = mx_lltoa(file_info->stat.st_size);
        size_len = mx_strlen(wsize); // Calculate size length for non-human-readable format
        free(wsize);
    }

    return size_len;
}

// Function to calculate and update the maximum widths of various fields
static void calculate_and_update_widths(t_width *width, t_file_info *file_info, t_configuration *configuration) {
    char *nlinks = mx_lltoa(file_info->stat.st_nlink);
    calculate_field_width(&width->links, nlinks); // Calculate width for hard links field
    free(nlinks);

    calculate_field_width(&width->user, file_info->user); // Calculate width for user field
    calculate_field_width(&width->group, file_info->group); // Calculate width for group field

    int size_len = calculate_size_length(file_info, configuration);
    calculate_field_width(&width->size, mx_itoa(size_len)); // Calculate width for size field
}

// Function to initialize width structure
static t_width initialize_width() {
    t_width width;
    width.links = 0;
    width.user = 0;
    width.group = 0;
    width.size = 0;
    return width;
}

// Function to calculate the maximum width of various fields for formatting
static t_width calculate_max_width(t_list *files_info, t_configuration *configuration) {
    t_width width = initialize_width();

    // Iterate through file information to determine maximum field widths
    while (files_info) {
        t_file_info *file_info = files_info->data;
        calculate_and_update_widths(&width, file_info, configuration);
        files_info = files_info->next;
    }

    return width;
}

// Function to print detailed information for multiple files with formatting based on maximum widths
void mx_print_with_info(t_list *files_info, t_configuration *configuration) {
    // Calculate the maximum width of various fields for formatting
    t_width max_width = calculate_max_width(files_info, configuration);

    // Loop through file information and print detailed information for each file
    while (files_info) {
        mx_print_file_info_detailed(files_info->data, &max_width, configuration);
        files_info = files_info->next;
    }
}
