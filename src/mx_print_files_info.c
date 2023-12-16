#include "../inc/uls.h"

// Function to sort a list of file information based on the specified sorting type
void mx_sort_file_list_info(t_list *files_info, t_sort_type sort_type, bool isReversed) {
    switch (sort_type) {
        case BY_NAME:
            mx_sort_list(files_info, compare_by_name, isReversed); // Sort list by name
            break;
        case BY_SIZE:
            mx_sort_list(files_info, compare_by_size, isReversed); // Sort list by size
            break;
        case BY_TIME:
            mx_sort_list(files_info, compare_by_time, isReversed); // Sort list by time
            break;
        default:
            break;
    }
}
