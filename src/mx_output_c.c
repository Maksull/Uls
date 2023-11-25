#include "uls.h"

static void print_cols(t_entity **name_arr, int rows, int num, int max_length) {
    for (int a = 0; a < rows; a++) {
        for (int j = 0; a + j < num; j += rows) {
            mx_printstr(name_arr[a + j]->name_str);

            if (name_arr[a + j + 1] && (a + j + rows < num)) {
                mx_print_tab(mx_strlen(name_arr[a + j]->name_str), max_length);
            }
        }
        if (a != rows - 1) {
            mx_printchar('\n');
        }
    }
}

static void print_name_arr(t_entity **name_arr, int max_length, int wincol) {
    int rows;
    int cols = (wincol / max_length) != 0 ? wincol / max_length : 1;
    int counter = 0;

    while (name_arr[counter]) {
        counter++;
    }

    if (max_length * cols > wincol && cols != 1) {
        cols--;
    }

    if (counter * max_length > wincol) {
        rows = counter / cols;
        if (rows == 0 || counter % cols != 0) {
            rows++;
        }
        print_cols(name_arr, rows, counter, max_length);
        mx_printchar('\n');
    } else {
        for (int a = 0; name_arr[a]; a++) {
            mx_printstr(name_arr[a]->name_str);
            if (name_arr[a + 1]) {
                mx_print_tab(mx_strlen(name_arr[a]->name_str), max_length);
            }
        }
        mx_printchar('\n');
    }
}

void mx_output_c(t_entity **name_arr) {
    struct winsize window;
    int max_length;

    if (!name_arr) {
        return;
    }

    max_length = mx_max_lenngth_name_arr(name_arr);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    isatty(1) ? print_name_arr(name_arr, max_length, window.ws_col) : print_name_arr(name_arr, max_length, 80);
}
