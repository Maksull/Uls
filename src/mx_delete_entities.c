#include "uls.h"

void mx_delete_entities(t_entity ***data, t_entity **dirs) {
    t_entity **arr_to_delete = *data;

    for (int i = 0; arr_to_delete[i]!= NULL; i++) {
        mx_strdel(&arr_to_delete[i]->name_str);
        mx_strdel(&arr_to_delete[i]->path_str);
        if (arr_to_delete[i]->error) {
            mx_strdel(&arr_to_delete[i]->error);
        }
        free(arr_to_delete[i]);
        arr_to_delete[i] = NULL;
    }
    free(*data);
    *data = dirs;
}
