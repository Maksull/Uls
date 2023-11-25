#include "uls.h"

t_entity *mx_create_new_file_node(t_entity *arg) {
    t_entity *new = (t_entity *)malloc(1 * sizeof (t_entity));

    new->name_str = mx_strdup(arg->name_str);
    new->path_str = mx_strdup(arg->path_str);
    if (arg->error) {
        new->error = mx_strdup(arg->error);
    } else {
        new->error = NULL;
    }

    lstat(new->path_str, &(new->info_st));

    if (arg->next != NULL) {
        new->next = arg->next;
    } else {
        new->next = NULL;
    } 

    return new;
}
