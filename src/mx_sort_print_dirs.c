#include "../inc/uls.h"

// Function to sort and print directories
bool mx_sort_print_dirs(t_list *dirs, t_configuration *configuration, bool must_print_names) {
    bool result = true;

    // Sort directories and print each directory
    mx_sort_file_list_info(dirs, configuration->sort_type, configuration->sort_reverse);
    while (dirs) {
        t_file_info *file_info = dirs->data;
        // Print and sort each directory, updating the result status
        if (!mx_sort_print_dir(file_info, configuration, must_print_names))
        {
            result = false;
        }
        if (dirs->next)
        {
            mx_printchar('\n');
        }
        dirs = dirs->next;
    }

    return result; // Return the result status of printing directories
}
