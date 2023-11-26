#include "../inc/uls.h"

// Recursively frees memory allocated for an array of t_object pointers
void mx_free_arr(t_object ***data) {
    t_object **arr_to_delete = *data;

    // Iterate through the array until a NULL pointer is encountered
    for (int a = 0; arr_to_delete[a] != NULL; a++) {
        // Free memory allocated for name and path strings of the current object
        mx_strdel(&arr_to_delete[a]->name);
        mx_strdel(&arr_to_delete[a]->path);

        // Check if an error message exists for the current object and free it
        if (arr_to_delete[a]->error)
            mx_strdel(&arr_to_delete[a]->error);

        // Recursively free memory for 'next' pointers if they exist
        if (arr_to_delete[a]->next != NULL)
            mx_free_arr(&arr_to_delete[a]->next);

        // Free memory allocated for the current object and set it to NULL
        free(arr_to_delete[a]);
        arr_to_delete[a] = NULL;
    }

    // Free memory allocated for the array of t_object pointers and set it to NULL
    free(*data);
    *data = NULL;
}
