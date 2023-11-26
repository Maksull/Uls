#include "../inc/uls.h"

// Creates a new object node based on the provided argument object
t_object *mx_create_new_file_node(t_object *arg) {
    // Allocate memory for the new object node
    t_object *new = (t_object *)malloc(1 * sizeof (t_object));

    // Copy the name and path from the argument object to the new node
    new->name = mx_strdup(arg->name);
    new->path = mx_strdup(arg->path);

    // Check if the argument object has an error message
    if (arg->error) {
        // If an error exists, duplicate it for the new node
        new->error = mx_strdup(arg->error);
    } else {
        // If no error exists, set the error of the new node to NULL
        new->error = NULL;
    }

    // Get file status information for the new node using lstat
    lstat(new->path, &(new->info_st));

    // Set the 'next' pointer of the new node based on the argument object
    if (arg->next != NULL) {
        // If the argument object has a 'next' node, assign it to the new node
        new->next = arg->next;
    } else {
        // If the argument object's 'next' pointer is NULL, set the new node's 'next' to NULL
        new->next = NULL;
    }

    // Return the newly created object node
    return new;
}
