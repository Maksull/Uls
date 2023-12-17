#include "../inc/uls.h"

// Function to calculate the maximum width among file names in the list
static int calculate_max_width(t_list *files_info) {
    int max_width = 0;
    while (files_info != NULL) {
        t_file_info *file_info = files_info->data;
        int length = mx_strlen(file_info->name);
        if (max_width < length)
        {
            max_width = length;
        }
            
        files_info = files_info->next;
    }

    return max_width; // Return the maximum width among file names
}

// Function to convert a list of file_info structures into an array of file_info pointers
static t_file_info **list_to_file_info_array(t_list *files_info, int *file_number) {
    *file_number = mx_list_size(files_info);
    if (*file_number == 0)
    {
        return NULL;
    }

    // Allocate memory for an array of file_info pointers and populate the array
    t_file_info **file_info_array = malloc(sizeof(t_file_info *) * (*file_number));
    for (int i = 0; i < *file_number; i++) {
        file_info_array[i] = files_info->data;
        files_info = files_info->next;
    }

    return file_info_array;
}

static int calculate_terminal_width() {
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }
    return terminal_width;
}

static int calculate_column_number(int width, int terminal_width) {
    int column_number = terminal_width / width;
    return (column_number <= 1) ? 1 : column_number;
}

static int calculate_row_number(int file_number, int column_number) {
    return (file_number + column_number - 1) / column_number;
}

static void print_file_info_with_tabs(t_file_info *file_info, int width, int tabwidth, t_configuration *configuration) {
    int printed = mx_print_file_info(file_info, configuration);
    int tabs = (width - printed + tabwidth - 1) / tabwidth;
    for (int i = 0; i < tabs; i++) {
        if (tabwidth == 1)
        {
            mx_printchar(' ');
        }
        else
        {
            mx_printchar('\t');
        }
            
    }
}

static void print_multi_column_rows(t_file_info **array, int row_number, int column_number, int width, int tabwidth, int file_number, t_configuration *configuration) {
    int index = 0;
    for (int i = 0; i < row_number; i++) {
        if (!configuration->sort_horizontally)
            index = i;
        for (int j = 0; j < column_number; j++) {
            print_file_info_with_tabs(array[index], width, tabwidth, configuration);
            if (configuration->sort_horizontally)
            {
                index++;
            }
            else
            {
                index += row_number;
            }
            if (index >= file_number)
            {
                break;
            }
        }
        mx_printchar('\n');
    }
}

// Function to print files in a multi-column format
void mx_print_multi_column(t_list *files_info, t_configuration *configuration) {
    int tabwidth = (configuration->use_colors) ? 1 : 8;
    int width = calculate_max_width(files_info);
    if (configuration->classify || configuration->add_only_slash_to_directories)
        width++;
    width = (width + tabwidth) & ~(tabwidth - 1);

    int terminal_width = calculate_terminal_width();
    int file_number;
    t_file_info **array = list_to_file_info_array(files_info, &file_number);
    int column_number = calculate_column_number(width, terminal_width);

    if (column_number <= 1) {
        mx_print_one_column(files_info, configuration);
        return;
    }

    int row_number = calculate_row_number(file_number, column_number);

    print_multi_column_rows(array, row_number, column_number, width, tabwidth, file_number, configuration);

    if (array)
    {
        free(array);
    }
}
