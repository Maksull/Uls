#include "uls.h"

static void print_spaces_g(int len, int maxlen) {
    for (int i = 0; i < maxlen - len; i++) {
        mx_printchar(' ');
    }
}

static void printcols_c(t_entity** name_arr, int rows, int numb, int maxlen) {
    for (int i = 0; i < rows; i++) {
        for (int j = i; j < numb; j += rows) {
            mx_printstr_with_color(name_arr[j]);
            if (name_arr[j + rows] && (j + rows < numb)) {
                print_spaces_g(mx_strlen(name_arr[j]->name_str), maxlen);
            }
        }
        if (i != rows - 1) {
            mx_printchar('\n');
        }
    }
}

static void printcols_x(t_entity** name_arr, int rows, int columns, int maxlen) {
    int col_num = columns;

    for (int i = 0; i < rows; i++, columns += col_num) {
        for (int j = 0; j < columns; j++) {
            if (name_arr[j] != NULL) {
                mx_printstr_with_color(name_arr[j]);
                if (name_arr[j + 1] && (j != columns - 1))
                    print_spaces_g(mx_strlen(name_arr[j]->name_str), maxlen);
            }
        }
        if (i != rows - 1) {
            mx_printchar('\n');
        }
    }
}

static int max_length(t_entity** name_arr) {
    int max = 0, tmp = 0;

    for (int i = 0; name_arr[i] != NULL; i++) {
        tmp = mx_strlen(name_arr[i]->name_str);
        max = tmp > max ? tmp : max;
    }
    return (max + 1);
}

static void print_name_arr(t_entity** name_arr, int maxlen, int width, t_flag* flag) {
    int rows, numb = 0;
    int cols = (width / maxlen) != 0 ? width / maxlen : 1;

    while (name_arr[numb] != NULL)
        numb++;
    if (cols != 1 && maxlen * cols > width)
        cols--;
    if (numb * maxlen > width) {
        rows = numb / cols;
        if (numb % cols != 0 || rows == 0) {
            rows += 1;
        }
        if (flag->has_X == 0) {
            printcols_c(name_arr, rows, numb, maxlen);
        }
        else {
            printcols_x(name_arr, rows, cols, maxlen);
        }
    }
    else {
        for (int i = 0; name_arr[i]; i++) {
            mx_printstr_with_color(name_arr[i]);
            if (name_arr[i + 1]) {
                print_spaces_g(mx_strlen(name_arr[i]->name_str), maxlen);
            }
        }
    }
    mx_printchar('\n');
}

void mx_output_with_color(t_entity** name_arr, t_flag* flag) {
    if (name_arr == NULL)
        return;

    struct winsize window;
    int maxlen = max_length(name_arr);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    print_name_arr(name_arr, maxlen, window.ws_col, flag);
}
