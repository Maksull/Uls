#include "uls.h"

int main(int argc, char *argv[]) {
    int counter = 1;
    t_flag *flag = mx_get_flags(argv, &counter);
    t_entity **data = mx_get_name_arr(argc, argv, counter);
    mx_sort_content(&data, flag);
    int exit_code = 0;

    if (data) {
        mx_opendir(&data, flag);
    }

    if (flag->exit_code == 1) {
        exit_code = 1;
    }

    free(flag);
    flag = NULL;

    exit(exit_code);
}
