#include "../inc/uls.h"

// Function to calculate the maximum width among file names in the list
static int calculate_max_width(t_list *files_info) {
    int max_width = 0;
    while (files_info != NULL) {
        t_file_info *file_info = files_info->data;
        int length = mx_strlen(file_info->name);
        if (max_width < length)
            max_width = length;
        files_info = files_info->next;
    }
    return max_width; // Return the maximum width among file names
}

// Function to convert a list of file_info structures into an array of file_info pointers
static t_file_info **list_to_file_info_array(t_list *files_info, int *file_number) {
    *file_number = mx_list_size(files_info);
    if (*file_number == 0)
        return NULL;

    // Allocate memory for an array of file_info pointers and populate the array
    t_file_info **file_info_array = malloc(sizeof(t_file_info *) * (*file_number));
    for (int i = 0; i < *file_number; i++) {
        file_info_array[i] = files_info->data;
        files_info = files_info->next;
    }

    return file_info_array; // Return the array of file_info pointers
}

// Function to print files in a multi-column format
void mx_print_multi_column(t_list *files_info, t_configuration *configuration) {
    int tabwidth = 8; // Default tab width
    if (configuration->use_colors)
        tabwidth = 1; // Set tab width to 1 if colors are used

    // Calculate the maximum width of file names and adjust for tabbing
    int width = calculate_max_width(files_info);
    if (configuration->classify || configuration->add_only_slash_to_directories)
        width++;
    width = (width + tabwidth) & ~(tabwidth - 1); // Ensure width is a multiple of tabwidth

    int terminal_width = 80; // Default terminal width
    if (isatty(1)) {
        // Retrieve terminal width if outputting to a terminal
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }

    int column_number = terminal_width / width; // Calculate the number of columns
    if (column_number <= 1) {
        mx_print_one_column(files_info, configuration); // If only one column can fit, print in single column format
        return;
    }

    int file_number;
    t_file_info **array = list_to_file_info_array(files_info, &file_number); // Convert list to an array
    int row_number = (file_number + column_number - 1) / column_number; // Calculate the number of rows

    int index = 0;
    for (int i = 0; i < row_number; i++) {
        if (!configuration->sort_horizontally)
            index = i; // Adjust index for horizontal sorting
        for (int j = 0; j < column_number; j++) {
            int printed = mx_print_file_info(array[index], configuration); // Print file information
            if (configuration->sort_horizontally)
                index++;
            else
                index += row_number;
            if (index >= file_number)
                break;
            int tabs = (width - printed + tabwidth - 1) / tabwidth; // Calculate the number of tabs for alignment
            for (int i = 0; i < tabs; i++) {
                if (tabwidth == 1)
                    mx_printchar(' ');
                else
                    mx_printchar('\t');
            }
        }
        mx_printchar('\n'); // Print a new line after each row
    }
    if (array != NULL)
        free(array); // Free allocated memory for the array
}
