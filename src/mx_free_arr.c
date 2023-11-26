#include "../inc/uls.h"

void mx_free_arr(t_object ***data) {
    t_object **arr_to_delete = *data;

    for (int a = 0; arr_to_delete[a]!= NULL; a++) {
        mx_strdel(&arr_to_delete[a]->name);
        mx_strdel(&arr_to_delete[a]->path);
        if (arr_to_delete[a]->error)
            mx_strdel(&arr_to_delete[a]->error);
        if (arr_to_delete[a]->next != NULL)
            mx_free_arr(&arr_to_delete[a]->next);
        free(arr_to_delete[a]);
        arr_to_delete[a] = NULL;
    }

    free(*data);
    *data = NULL;
}
