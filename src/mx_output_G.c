#include "../inc/uls.h"

// Print spaces to align columns
static void print_spaces(int len, int maxlen) {
    for (int i = 0; i < maxlen - len; i++) {
        mx_printchar(' ');
    }
}

// Print columns in a vertical arrangement
static void printcols_c(t_object** name_arr, int rows, int numb, int maxlen) {
    for (int i = 0; i < rows; i++) {
        for (int j = i; j < numb; j += rows) {
            mx_printstr_G(name_arr[j]); // Print with colored output
            if (name_arr[j + rows] && (j + rows < numb)) {
                print_spaces(mx_strlen(name_arr[j]->name), maxlen); // Align columns with spaces
            }
        }
        if (i != rows - 1) {
            mx_printchar('\n');
        }
    }
}

// Print columns in a horizontal arrangement
static void print_cols_x(t_object** name_arr, int rows, int columns, int maxlen) {
    int col_num = columns;

    for (int i = 0; i < rows; i++, columns += col_num) {
        for (int j = 0; j < columns; j++) {
            if (name_arr[j] != NULL) {
                mx_printstr_G(name_arr[j]); // Print with colored output
                if (name_arr[j + 1] && (j != columns - 1)) {
                    print_spaces(mx_strlen(name_arr[j]->name), maxlen); // Align columns with spaces
                }
            }
        }
        if (i != rows - 1) {
            mx_printchar('\n');
        }
    }
}

// Calculate the maximum length among the names
static int max_length(t_object** name_arr) {
    int max = 0, tmp = 0;

    for (int i = 0; name_arr[i] != NULL; i++) {
        tmp = mx_strlen(name_arr[i]->name);
        max = tmp > max ? tmp : max;
    }
    return (max + 1);
}

// Print the name_arr with proper formatting based on the terminal width
static void print_name_arr(t_object** name_arr, int maxlen, int width, t_flag* flag) {
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
        if (flag->X == 0) {
            printcols_c(name_arr, rows, numb, maxlen); // Print columns in a vertical arrangement
        }
        else {
            print_cols_x(name_arr, rows, cols, maxlen); // Print columns in a horizontal arrangement
        }
    }
    else {
        for (int i = 0; name_arr[i]; i++) {
            mx_printstr_G(name_arr[i]); // Print with colored output
            if (name_arr[i + 1]) {
                print_spaces(mx_strlen(name_arr[i]->name), maxlen); // Align columns with spaces
            }
        }
    }
    mx_printchar('\n');
}

// Output function for colored output based on flag
void mx_output_G(t_object** name_arr, t_flag* flag) {
    if (name_arr == NULL)
        return;

    struct winsize window;
    int maxlen = max_length(name_arr);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    print_name_arr(name_arr, maxlen, window.ws_col, flag); // Print name_arr with proper formatting
}
