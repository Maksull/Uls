#include "../inc/uls.h"

// Prints the columns based on the given rows and the maximum length for formatting
static void print_cols(t_object **name_arr, int rows, int num, int max_length) {
    for (int a = 0; a < rows; a++) {
        for (int j = 0; a + j < num; j += rows) {
            mx_printstr(name_arr[a + j]->name); // Print the file/directory name

            // If there's another name in the same column, add tabs for alignment
            if (name_arr[a + j + 1] && (a + j + rows < num)) {
                mx_print_tab(mx_strlen(name_arr[a + j]->name), max_length);
            }
        }
        if (a != rows - 1) {
            mx_printchar('\n'); // Print new line unless it's the last row
        }
    }
}

// Prints the name array considering the maximum length and window column size
static void print_name_arr(t_object **name_arr, int max_length, int wincol) {
    int rows;
    // Calculate the number of columns needed to fit in the window size
    int cols = (wincol / max_length) != 0 ? wincol / max_length : 1;
    int counter = 0;

    // Count the number of elements in the name array
    while (name_arr[counter]) {
        counter++;
    }

    // Adjust columns if they exceed the window size but more than one column is present
    if (max_length * cols > wincol && cols != 1) {
        cols--;
    }

    // Calculate rows needed and print columns accordingly if contents exceed window size
    if (counter * max_length > wincol) {
        rows = counter / cols;
        if (rows == 0 || counter % cols != 0) {
            rows++;
        }
        print_cols(name_arr, rows, counter, max_length);
        mx_printchar('\n'); // Print new line after column output
    } else {
        // If contents fit in a single row, print the names horizontally
        for (int a = 0; name_arr[a]; a++) {
            mx_printstr(name_arr[a]->name);
            if (name_arr[a + 1]) {
                mx_print_tab(mx_strlen(name_arr[a]->name), max_length);
            }
        }
        mx_printchar('\n'); // Print new line after row output
    }
}

// Outputs the names in columns or a single row based on terminal window size or default width
void mx_output_c(t_object **name_arr) {
    struct winsize window;
    int max_length;

    if (!name_arr) {
        return; // Exit if the name array is empty
    }

    max_length = mx_get_max_length_name(name_arr); // Calculate the maximum name length
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window); // Fetch terminal window size

    // Choose to print in columns or single row based on terminal width or default width
    isatty(1) ? print_name_arr(name_arr, max_length, window.ws_col) : print_name_arr(name_arr, max_length, 80);
}
