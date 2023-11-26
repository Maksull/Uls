#include "../inc/uls.h"

// Handles the output based on the data and flag parameters
static void handle_output(t_object **data, t_flag *flag) {
    // If there are more nodes connected to the current one
    if ((*data)->next != NULL) {
        mx_output_selector(&(*data)->next, flag, 1); // Output selector for connected nodes
        if (flag->recursion == 1) {
            flag->files = 1; // Set flag for files
            mx_free_files(&(*data)->next, flag); // Free memory for connected nodes
            // If there are still connected nodes, process the directory
            if ((*data)->next) {
                mx_printstr("\n");
                mx_process_dir(&(*data)->next, flag);
            }
        }
    } else if ((*data)->error != NULL) { // If there's an error associated with the node
        mx_printerr("uls: ");
        mx_printerr((*data)->path);
        mx_printerr(": ");
        mx_printerr((*data)->error);
        mx_printerr("\n");
    }
}

// Outputs default information based on the data and flag parameters
void mx_output_default(t_object ***data, t_flag *flag) {
    if (*data) { // Check if there is data to process
        for (int a = 0; (*data)[a] != NULL; a++) {
            if (flag->files == 1) { // If files flag is set
                // Print the path or file name with formatting
                if ((*data)[a]->path[0] == '/' && (*data)[a]->path[1] == '/') {
                    mx_printstr(&(*data)[a]->path[1]);
                } else {
                    mx_printstr((*data)[a]->path);
                }
                mx_printstr(":"); // Separator
                mx_printstr("\n");
            }

            handle_output(&(*data)[a], flag); // Handle output based on data and flags

            // Print new line if files flag is set and there's more data
            if (flag->files == 1 && (*data)[a + 1]) {
                mx_printstr("\n");
            }
        }

        mx_free_arr(data); // Free memory allocated for data
    }
}
