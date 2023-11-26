#include "../inc/uls.h"

static void handle_output(t_object **data, t_flag *flag) {
    if ((*data)->next != NULL) {
        mx_output_selector(&(*data)->next, flag, 1);
        if (flag->recursion == 1) {
            flag->files = 1;
            mx_free_files(&(*data)->next, flag);
            if ((*data)->next) {
                mx_printstr("\n");
                mx_process_dir(&(*data)->next, flag);
            }
        }
    } else if ((*data)->error != NULL) {
        mx_printerr("uls: ");
        mx_printerr((*data)->path);
        mx_printerr(": ");
        mx_printerr((*data)->error);
        mx_printerr("\n");
    }
}

void mx_output_default(t_object ***data, t_flag *flag) {
    if (*data) {
        for (int a = 0; (*data)[a] != NULL; a++) {
            if (flag->files == 1) {
                if ((*data)[a]->path[0] == '/' && (*data)[a]->path[1] == '/') {
                    mx_printstr(&(*data)[a]->path[1]);
                } else {
                    mx_printstr((*data)[a]->path);
                }
                mx_printstr(":");
				mx_printstr("\n");
            }

            handle_output(&(*data)[a], flag);

            if (flag->files == 1 && (*data)[a+1]) {
                mx_printstr("\n");
            }
        }

        mx_free_arr(data);
    }
}
