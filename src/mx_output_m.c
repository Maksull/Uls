#include "../inc/uls.h"

static void print_name_arr(t_object **name_arr, int width, t_flag *flag) {
    int len = 0, next = 0, sum;

    for (int i = 0; name_arr[i] != NULL; i++) {
        if (flag->G != 1)
            mx_printstr(name_arr[i]->name);
        else if (flag->G == 1)
            mx_printstr_G(name_arr[i]);
        len += mx_strlen(name_arr[i]->name) + 2;
        if (name_arr[i + 1]) {
            mx_printstr(", ");
            next = name_arr[i + 2] ? 3 : 1;
            sum = len + next + mx_strlen(name_arr[i + 1]->name);
            if (sum > width) {
                len = 0;
                mx_printchar('\n');
            }
        }
    }
    mx_printchar('\n');
}

void mx_output_m(t_object **name_arr, t_flag *flag) {
    if (name_arr == NULL)
        return;

    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    int width = isatty(STDOUT_FILENO) ? window.ws_col : 80;
    print_name_arr(name_arr, width, flag);
}
