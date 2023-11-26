#include "../inc/uls.h"

void mx_print_wrong_usage(char flag) {
    mx_printerr("uls: illegal option -- ");
    write(2, &flag, 1);
    mx_printerr("\n");
    mx_printerr("usage: uls [-");
    mx_printerr(FLAGS);
    mx_printerr("] [file ...]\n");
}

void mx_print_invalid_dir(t_object ***err, t_flag *flag) {
    if (err && *err && **err) {
        mx_sort_objects(err, flag);
        flag->files = 1;
        flag->exit_code = 1;
        for (int i = 0; (*err)[i]; i++) {
            char *error = mx_strjoin(mx_strjoin("uls: ", (*err)[i]->name), ": ");
            error = mx_strjoin(mx_strjoin(error, (*err)[i]->error), "\n");
            mx_printerr(error);
            mx_strdel(&(*err)[i]->name);
            mx_strdel(&(*err)[i]->path);
            mx_strdel(&(*err)[i]->error);
            free((*err)[i]);
            (*err)[i] = NULL;
        }
        free(*err);
        *err = NULL;
    }
}
