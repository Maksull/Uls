#include "../inc/uls.h"

void mx_free_entities(t_object ***data, t_object **dirs) {
    t_object **arr_to_delete = *data;

    for (int i = 0; arr_to_delete[i]!= NULL; i++) {
        mx_strdel(&arr_to_delete[i]->name);
        mx_strdel(&arr_to_delete[i]->path);
        if (arr_to_delete[i]->error) {
            mx_strdel(&arr_to_delete[i]->error);
        }
        free(arr_to_delete[i]);
        arr_to_delete[i] = NULL;
    }
    free(*data);
    *data = dirs;
}
