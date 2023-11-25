#include "uls.h"

static void handle_output(t_entity **data, t_flag *flag) {
    if ((*data)->next != NULL) {
        mx_output_selector(&(*data)->next, flag, 1);
        if (flag->has_recursion == 1) {
            flag->has_files = 1;
            mx_delete_files(&(*data)->next, flag);
            if ((*data)->next) {
                mx_printstr("\n");
                mx_opendir(&(*data)->next, flag);
            }
        }
    } else if ((*data)->error != NULL) {
        mx_printerr("uls: ");
        mx_printerr((*data)->path_str);
        mx_printerr(": ");
        mx_printerr((*data)->error);
        mx_printerr("\n");
    }
}

void mx_output_default(t_entity ***data, t_flag *flag) {
    if (*data) {
        for (int a = 0; (*data)[a] != NULL; a++) {
            if (flag->has_files == 1) {
                if ((*data)[a]->path_str[0] == '/' && (*data)[a]->path_str[1] == '/') {
                    mx_printstr(&(*data)[a]->path_str[1]);
                } else {
                    mx_printstr((*data)[a]->path_str);
                }
                mx_printstr(":");
				mx_printstr("\n");
            }

            handle_output(&(*data)[a], flag);

            if (flag->has_files == 1 && (*data)[a+1]) {
                mx_printstr("\n");
            }
        }

        mx_delete_arr(data);
    }
}
