#include "../inc/uls.h"

// Print names from the array separated by commas, wrapping if needed based on terminal width
static void print_name_arr(t_object **name_arr, int width, t_flag *flag) {
    int len = 0, next = 0, sum;

    // Iterate through the name array
    for (int i = 0; name_arr[i] != NULL; i++) {
        // Print the name based on the flag set (G or not)
        if (flag->G != 1)
            mx_printstr(name_arr[i]->name);
        else if (flag->G == 1)
            mx_printstr_G(name_arr[i]);

        // Calculate the length, add for separator and next name length
        len += mx_strlen(name_arr[i]->name) + 2;

        // Check if next name exists
        if (name_arr[i + 1]) {
            mx_printstr(", ");
            // Set next based on next name's existence
            next = name_arr[i + 2] ? 3 : 1;
            sum = len + next + mx_strlen(name_arr[i + 1]->name);
            // If sum exceeds terminal width, start a new line
            if (sum > width) {
                len = 0;
                mx_printchar('\n');
            }
        }
    }
    mx_printchar('\n');
}

// Output function for listing names in comma-separated format
void mx_output_m(t_object **name_arr, t_flag *flag) {
    if (name_arr == NULL)
        return;

    // Get terminal width
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    int width = isatty(STDOUT_FILENO) ? window.ws_col : 80;

    // Print the name array in comma-separated format based on terminal width
    print_name_arr(name_arr, width, flag);
}
