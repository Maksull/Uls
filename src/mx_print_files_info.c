#include "../inc/uls.h"

// Function to determine the printing format and call the corresponding printing function
void mx_print_files_info(t_list *files_info, t_configuration *configuration) {
    switch (configuration->format) {
        case ONE_COLUMN:
            mx_print_one_column(files_info, configuration); // Print files in a single column
            break;
        case MULTI_COLUMN:
            mx_print_multi_column(files_info, configuration); // Print files in multiple columns
            break;
        case STREAM:
            mx_print_stream(files_info, configuration); // Print files in a stream
            break;
        case DETAILED:
            mx_print_with_info(files_info, configuration); // Print detailed information for files
            break;
        default:
            break;
    }
}
