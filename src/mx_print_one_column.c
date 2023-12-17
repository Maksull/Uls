#include "../inc/uls.h"

// Function to print file information in a single column
void mx_print_one_column(t_list *files_info, t_configuration *configuration) {
    while (files_info) {
        mx_print_file_info(files_info->data, configuration); // Print file information
        mx_printchar('\n'); // Move to the next line for the next file
        files_info = files_info->next; // Move to the next file in the list
    }
}
