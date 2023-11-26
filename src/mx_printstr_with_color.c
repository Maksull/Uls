#include "../inc/uls.h"

static void print_name(t_object* data) {
    mx_printstr(data->name);
    mx_printstr("\x1b[0m"); // reset color
}

static int print_first(t_object* data) {
    mode_t mode = data->info_st.st_mode;
    if (IS_DIR(mode)) {
        mx_printstr("\033[34m");
        print_name(data);
        return 1;
    }
    else if (mode & S_IXOTH) {
        mx_printstr("\x1b[31m");
        print_name(data);
        return 1;
    }
    else if (IS_LNK(mode)) {
        mx_printstr("\033[35m");
        print_name(data);
        return 1;
    }
    return 0;
}

static int print_second(t_object* data) {
    mode_t mode = data->info_st.st_mode;
    if (IS_BLK(mode)) {
        mx_printstr("\033[34;46m");
        print_name(data);
        return 1;
    }
    else if (IS_SOCK(mode)) {
        mx_printstr("\033[32m");
        print_name(data);
        return 1;
    }
    else if (IS_CHR(mode)) {
        mx_printstr("\033[34;43m");
        print_name(data);
        return 1;
    }
    return 0;
}

void mx_printstr_G(t_object* data) {
    mode_t mode = data->info_st.st_mode;

    if (print_first(data) == 1)
        return;
    if (print_second(data) == 1)
        return;

    if (IS_FIFO(mode))
        mx_printstr("\033[33m");
    else if (IS_EXEC(mode))
        mx_printstr("\033[31m");
    else if (IS_WHT(mode))
        mx_printstr("\033[36m");
    else {
        mx_printstr(data->name);
        return;
    }
    print_name(data);
}
