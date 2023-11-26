#include "../inc/uls.h"

int main(int argc, char *argv[]) {
    int counter = 1;
    t_flag *flag = mx_get_flags_applied(argv, &counter);
    t_object **data = mx_get_name_arr(argc, argv, counter);
    mx_sort_objects(&data, flag);
    int exit_code = 0;

    if (data) {
        mx_process_dir(&data, flag);
    }

    if (flag->exit_code == 1) {
        exit_code = 1;
    }

    free(flag);
    flag = NULL;

    exit(exit_code);
}
