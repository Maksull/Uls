#include "../inc/uls.h"

static void create_objects_for_delete(t_object ***dirs, t_object ***data) {
    int dir_count = 0;

    for (int i = 0; (*data)[i] != NULL; i++) {
        if ((*data)[i]->error == NULL && IS_DIR((*data)[i]->info_st.st_mode) && mx_strcmp((*data)[i]->name, ".") != 0 && mx_strcmp((*data)[i]->name, "..") != 0) {
            dir_count++;
        }
    }

    if (dir_count > 0) {
        *dirs = malloc((dir_count + 1) * sizeof(t_object *));
        if (!*dirs) {
            return;
        }
    }
}

static t_object *create_new_file_node_for_delete(t_object *arg) {
    t_object *new_node = (t_object *)malloc(1 * sizeof (t_object));

    new_node->error = NULL;
    new_node->path = mx_strdup(arg->path);
    new_node->name = mx_strdup(arg->name);
    if (arg->error != NULL) {
        new_node->error = mx_strdup(arg->error);
    }

    lstat(new_node->path, &(new_node->info_st));
    
    if (arg->next != NULL) {
        new_node->next = arg->next;
    } else {
        new_node->next = NULL;
    }

    return new_node;
}

void mx_free_files(t_object ***data, t_flag *flag) {
    t_object **dir_arr = NULL;
    int dir_count = 0;

    create_objects_for_delete(&dir_arr, data);

    for (int i = 0; (*data)[i] != NULL; i++) {
        t_object *current_entry = (*data)[i];

        if (current_entry->error == NULL && IS_DIR(current_entry->info_st.st_mode) &&
            mx_strcmp(current_entry->name, ".") != 0 &&
            mx_strcmp(current_entry->name, "..") != 0) {
            
            dir_arr[dir_count++] = create_new_file_node_for_delete(current_entry);
            dir_arr[dir_count] = NULL;
        }
    }

    flag->files = 1;
    mx_free_entities(data, dir_arr);
}
