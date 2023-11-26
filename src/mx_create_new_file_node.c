#include "../inc/uls.h"

t_object *mx_create_new_file_node(t_object *arg) {
    t_object *new = (t_object *)malloc(1 * sizeof (t_object));

    new->name = mx_strdup(arg->name);
    new->path = mx_strdup(arg->path);
    if (arg->error) {
        new->error = mx_strdup(arg->error);
    } else {
        new->error = NULL;
    }

    lstat(new->path, &(new->info_st));

    if (arg->next != NULL) {
        new->next = arg->next;
    } else {
        new->next = NULL;
    } 

    return new;
}
