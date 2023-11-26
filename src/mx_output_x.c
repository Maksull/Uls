#include "../inc/uls.h"

// Prints the objects in columns and rows horizontally
static void print_cols_x(t_object** name_arr, int rows, int cols, int maxlen) {
    int tmpcols = cols;

    // Loop through rows
    for (int i = 0; i < rows; cols += tmpcols, i++) {
        // Loop through columns
        for (int j = 0; j < cols; j++) {
            // Print the object name
            if (name_arr[j] != NULL) {
                mx_printstr(name_arr[j]->name);
                // Add tabs between names unless it's the last name in the row
                if ((j != cols - 1) && name_arr[j + 1] != NULL)
                    mx_print_tab(mx_strlen(name_arr[j]->name), maxlen);
            }
        }
        // Print a newline unless it's the last row
        if (i != rows - 1) {
            mx_printchar('\n');
        }
    }
}

// Prints the names of the objects in a horizontally sorted format
static void print_name_arr(t_object** name_arr, int max, int width) {
    int rows, num = 0;
    int columns = (width / max) != 0 ? (width / max) : 1;

    // Count the number of objects in the array
    while (name_arr[num] != NULL)
        num++;
    // Adjust the number of columns based on available width
    if (columns != 1 && (max * columns > width))
        columns--;
    // If the total width exceeds the available width, print in rows and columns
    if (num * max > width) {
        rows = num / columns;
        if (num % columns != 0 || rows == 0)
            rows++;
        print_cols_x(name_arr, rows, columns, max);
    } else {
        // Print objects horizontally
        for (int i = 0; name_arr[i] != NULL; i++) {
            mx_printstr(name_arr[i]->name);
            if (name_arr[i + 1] != NULL)
                mx_print_tab(mx_strlen(name_arr[i]->name), max);
        }
        mx_printchar('\n');
    }
}

// Selects and prints the object names in a horizontally sorted format
void mx_output_x(t_object** name_arr) {
    if (name_arr == NULL)
        return;

    // Get the maximum length of the object names
    int maxlen = mx_get_max_length_name(name_arr);
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    int width = isatty(STDOUT_FILENO) ? window.ws_col : 80;
    // Print the object names in a horizontally sorted format
    print_name_arr(name_arr, maxlen, width);
}
