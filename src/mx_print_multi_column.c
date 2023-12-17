#include "../inc/uls.h"

static int calculate_max_width(t_list *files_info) {
    int max_width = 0;
    while (files_info != NULL) {
        t_file_info *file_info = files_info->data;
        int length = mx_strlen(file_info->name);
        if (max_width < length)
            max_width = length;
        files_info = files_info->next;
    }
    return max_width;
}

static t_file_info **list_to_file_info_array(t_list *files_info, int *file_number) {
    *file_number = mx_list_size(files_info);
    if (*file_number == 0)
        return NULL;

    t_file_info **file_info_array = malloc(sizeof(t_file_info *) * (*file_number));
    for (int i = 0; i < *file_number; i++) {
        file_info_array[i] = files_info->data;
        files_info = files_info->next;
    }

    return file_info_array;
}

static int get_terminal_width() {
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }
    return terminal_width;
}

static int calculate_columns(int terminal_width, int width) {
    int column_number = terminal_width / width;
    return column_number;
}

static int calculate_rows(int file_number, int column_number) {
    int row_number = (file_number + column_number - 1) / column_number;
    return row_number;
}

static void print_file_info_row(t_file_info **array, int start_index, int end_index, int width, int tabwidth, t_configuration *configuration) {
    for (int i = start_index; i < end_index; i++) {
        int printed = mx_print_file_info(array[i], configuration);
        int tabs = (width - printed + tabwidth - 1) / tabwidth;
        for (int i = 0; i < tabs; i++) {
            if (tabwidth == 1)
                mx_printchar(' ');
            else
                mx_printchar('\t');
        }
    }
}

int calculate_tab_width(t_configuration *configuration) {
    return (configuration->use_colors) ? 1 : 8;
}

int calculate_width(t_list *files_info, t_configuration *configuration, int *tabwidth) {
    int width = calculate_max_width(files_info);
    if (configuration->classify || configuration->add_only_slash_to_directories)
        width++;
    return (width + *tabwidth) & ~(*tabwidth - 1);
}

void print_multi_column(t_file_info **array, int file_number, int row_number, int column_number, int width, int tabwidth, t_configuration *configuration) {
    int index = 0;
    for (int i = 0; i < row_number; i++) {
        if (!configuration->sort_horizontally)
            index = i;
        print_file_info_row(array, index, file_number, column_number, width, tabwidth, configuration);
        if (configuration->sort_horizontally)
            index++;
        else
            index += row_number;
        if (index >= file_number)
            break;
        mx_printchar('\n');
    }
}

void free_array(t_file_info **array) {
    if (array != NULL)
        free(array);
}

void mx_print_multi_column(t_list *files_info, t_configuration *configuration) {
    int tabwidth = calculate_tab_width(configuration);
    int width = calculate_width(files_info, configuration, &tabwidth);
    int terminal_width = get_terminal_width();
    int column_number = calculate_columns(terminal_width, width);

    if (column_number <= 1) {
        mx_print_one_column(files_info, configuration);
        return;
    }

    int file_number;
    t_file_info **array = list_to_file_info_array(files_info, &file_number);
    int row_number = calculate_rows(file_number, column_number);

    print_multi_column(array, file_number, row_number, column_number, width, tabwidth, configuration);

    free_array(array);
}


