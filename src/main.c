#include "../inc/uls.h"

int main(int argc, char *argv[]) {
    int counter = 1;

    // Get flags applied from command line arguments
    t_flag *flag = mx_get_flags_applied(argv, &counter);

    // Get names of files and directories from command line arguments
    t_object **data = mx_get_name_arr(argc, argv, counter);

    // Sort objects (files and directories) based on specified flags
    mx_sort_objects(&data, flag);

    int exit_code = 0;

    // Process directories and files
    if (data) {
        mx_process_dir(&data, flag);
    }

    // Set exit code based on flag
    if (flag->exit_code == 1) {
        exit_code = 1;
    }

    // Free allocated memory for flag
    free(flag);
    flag = NULL;

    // Exit with the specified exit code
    exit(exit_code);
}
