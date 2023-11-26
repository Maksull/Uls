#include "../inc/uls.h"

// Frees memory allocated for an array of t_object pointers and assigns 'dirs' to 'data'
void mx_free_entities(t_object ***data, t_object **dirs) {
    t_object **arr_to_delete = *data;

    // Iterate through the array until a NULL pointer is encountered
    for (int i = 0; arr_to_delete[i] != NULL; i++) {
        // Free memory allocated for name and path strings of the current object
        mx_strdel(&arr_to_delete[i]->name);
        mx_strdel(&arr_to_delete[i]->path);

        // Check if an error message exists for the current object and free it
        if (arr_to_delete[i]->error) {
            mx_strdel(&arr_to_delete[i]->error);
        }

        // Free memory allocated for the current object and set it to NULL
        free(arr_to_delete[i]);
        arr_to_delete[i] = NULL;
    }

    // Free memory allocated for the array of t_object pointers
    free(*data);

    // Assign 'dirs' to 'data' pointer, effectively passing 'dirs' out of the function
    *data = dirs;
}
