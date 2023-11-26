#include "../inc/uls.h"

// Creates objects to store directories for deletion from the data array
static void create_objects_for_delete(t_object ***dirs, t_object ***data) {
    int dir_count = 0;

    // Count the number of directories (excluding "." and "..") in the data array
    for (int i = 0; (*data)[i] != NULL; i++) {
        if ((*data)[i]->error == NULL && IS_DIR((*data)[i]->info_st.st_mode) &&
            mx_strcmp((*data)[i]->name, ".") != 0 && mx_strcmp((*data)[i]->name, "..") != 0) {
            dir_count++;
        }
    }

    // Allocate memory for storing directories to be deleted if there are any
    if (dir_count > 0) {
        *dirs = malloc((dir_count + 1) * sizeof(t_object *));
        if (!*dirs) {
            return; // Return if memory allocation fails
        }
    }
}

// Creates a new object node for deletion based on the provided argument object
static t_object *create_new_file_node_for_delete(t_object *arg) {
    t_object *new_node = (t_object *)malloc(1 * sizeof (t_object));

    // Initialize fields of the new node
    new_node->error = NULL;
    new_node->path = mx_strdup(arg->path);
    new_node->name = mx_strdup(arg->name);

    // Duplicate error message if it exists in the argument object
    if (arg->error != NULL) {
        new_node->error = mx_strdup(arg->error);
    }

    // Get file status information for the new node using lstat
    lstat(new_node->path, &(new_node->info_st));
    
    // Set the 'next' pointer of the new node based on the argument object
    if (arg->next != NULL) {
        new_node->next = arg->next;
    } else {
        new_node->next = NULL;
    }

    return new_node;
}

// Frees memory allocated for files and directories and updates flag values
void mx_free_files(t_object ***data, t_flag *flag) {
    t_object **dir_arr = NULL;
    int dir_count = 0;

    // Create objects to store directories for deletion from the data array
    create_objects_for_delete(&dir_arr, data);

    // Iterate through the data array to identify directories for deletion
    for (int i = 0; (*data)[i] != NULL; i++) {
        t_object *current_entry = (*data)[i];

        // Identify directories (excluding "." and "..") for deletion and store them in dir_arr
        if (current_entry->error == NULL && IS_DIR(current_entry->info_st.st_mode) &&
            mx_strcmp(current_entry->name, ".") != 0 &&
            mx_strcmp(current_entry->name, "..") != 0) {
            
            dir_arr[dir_count++] = create_new_file_node_for_delete(current_entry);
            dir_arr[dir_count] = NULL;
        }
    }

    // Set flag values indicating the existence of files for deletion
    flag->files = 1;

    // Free memory for files and directories using mx_free_entities
    mx_free_entities(data, dir_arr);
}
