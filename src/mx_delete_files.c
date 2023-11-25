#include "uls.h"

static void create_entities_for_delete(t_entity ***dirs, t_entity ***data) {
    int dir_count = 0;

    for (int i = 0; (*data)[i] != NULL; i++) {
        if ((*data)[i]->error == NULL && IS_DIR((*data)[i]->info_st.st_mode) && mx_strcmp((*data)[i]->name_str, ".") != 0 && mx_strcmp((*data)[i]->name_str, "..") != 0) {
            dir_count++;
        }
    }

    if (dir_count > 0) {
        *dirs = malloc((dir_count + 1) * sizeof(t_entity *));
        if (!*dirs) {
            return;
        }
    }
}

static t_entity *create_new_file_node_for_delete(t_entity *arg) {
    t_entity *new_node = (t_entity *)malloc(1 * sizeof (t_entity));

    new_node->error = NULL;
    new_node->path_str = mx_strdup(arg->path_str);
    new_node->name_str = mx_strdup(arg->name_str);
    if (arg->error != NULL) {
        new_node->error = mx_strdup(arg->error);
    }

    lstat(new_node->path_str, &(new_node->info_st));
    
    if (arg->next != NULL) {
        new_node->next = arg->next;
    } else {
        new_node->next = NULL;
    }

    return new_node;
}

void mx_delete_files(t_entity ***data, t_flag *flag) {
    t_entity **dir_arr = NULL;
    int dir_count = 0;

    create_entities_for_delete(&dir_arr, data);

    for (int i = 0; (*data)[i] != NULL; i++) {
        t_entity *current_entry = (*data)[i];

        if (current_entry->error == NULL && IS_DIR(current_entry->info_st.st_mode) &&
            mx_strcmp(current_entry->name_str, ".") != 0 &&
            mx_strcmp(current_entry->name_str, "..") != 0) {
            
            dir_arr[dir_count++] = create_new_file_node_for_delete(current_entry);
            dir_arr[dir_count] = NULL;
        }
    }

    flag->has_files = 1;
    mx_delete_entities(data, dir_arr);
}
