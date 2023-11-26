#include "../inc/uls.h"

static void print_cols_x(t_object** name_arr, int rows, int cols, int maxlen) {
    int tmpcols = cols;

    for (int i = 0; i < rows; cols += tmpcols, i++) {
        for (int j = 0; j < cols; j++) {
            if (name_arr[j] != NULL) {
                mx_printstr(name_arr[j]->name);
                if ((j != cols - 1) && name_arr[j + 1] != NULL)
                    mx_print_tab(mx_strlen(name_arr[j]->name), maxlen);
            }
        }
        if (i != rows - 1) {
            mx_printchar('\n');
        }
    }
}

static void print_name_arr(t_object** name_arr, int max, int width) {
    int rows, num = 0;
    int columns = (width / max) != 0 ? (width / max) : 1;

    while (name_arr[num] != NULL)
        num++;
    if (columns != 1 && (max * columns > width))
        columns--;
    if (num * max > width) {
        rows = num / columns;
        if (num % columns != 0 || rows == 0)
            rows++;
        print_cols_x(name_arr, rows, columns, max);
    } else {
        for (int i = 0; name_arr[i] != NULL; i++) {
            mx_printstr(name_arr[i]->name);
            if (name_arr[i + 1] != NULL)
                mx_print_tab(mx_strlen(name_arr[i]->name), max);
        }
        mx_printchar('\n');
    }
}

void mx_output_x(t_object** name_arr) {
    if (name_arr == NULL)
        return;

    int maxlen = mx_get_max_length_name(name_arr);
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    int width = isatty(STDOUT_FILENO) ? window.ws_col : 80;
    print_name_arr(name_arr, maxlen, width);
}
