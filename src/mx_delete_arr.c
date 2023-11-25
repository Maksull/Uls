#include "uls.h"

void mx_delete_arr(t_entity ***data) {
    t_entity **arr_to_delete = *data;

    for (int a = 0; arr_to_delete[a]!= NULL; a++) {
        mx_strdel(&arr_to_delete[a]->name_str);
        mx_strdel(&arr_to_delete[a]->path_str);
        if (arr_to_delete[a]->error)
            mx_strdel(&arr_to_delete[a]->error);
        if (arr_to_delete[a]->next != NULL)
            mx_delete_arr(&arr_to_delete[a]->next);
        free(arr_to_delete[a]);
        arr_to_delete[a] = NULL;
    }

    free(*data);
    *data = NULL;
}
