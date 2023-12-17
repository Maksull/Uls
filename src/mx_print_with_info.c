#include "../inc/uls.h"

// Function to print detailed information for multiple files
void mx_print_with_info(t_list *files_info, t_configuration *configuration) {
    // Calculate the maximum width of various fields for formatting
    t_width max_width = mx_calculate_max_width(files_info, configuration);

    // Loop through file information and print detailed information for each file
    while (files_info != NULL) {
        mx_print_file_info_detailed(files_info->data, &max_width, configuration);
        files_info = files_info->next;
    }
}
