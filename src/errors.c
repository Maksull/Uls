#include "uls.h"

void mx_print_flag_error(char flag) {
    mx_printerr("uls: illegal option -- ");
    write(2, &flag, 1);
    mx_printerr("\n");
    mx_printerr("usage: uls [-");
    mx_printerr(VALID_FLAGS);
    mx_printerr("] [file ...]\n");
}

void mx_handle_error(t_entity ***err, t_flag *flag) {
    if (err && *err && **err) {
        mx_sort_content(err, flag);
        flag->has_files = 1;
        flag->exit_code = 1;
        for (int i = 0; (*err)[i]; i++) {
            char *error = mx_strjoin(mx_strjoin("uls: ", (*err)[i]->name_str), ": ");
            error = mx_strjoin(mx_strjoin(error, (*err)[i]->error), "\n");
            mx_printerr(error);
            mx_strdel(&(*err)[i]->name_str);
            mx_strdel(&(*err)[i]->path_str);
            mx_strdel(&(*err)[i]->error);
            free((*err)[i]);
            (*err)[i] = NULL;
        }
        free(*err);
        *err = NULL;
    }
}
