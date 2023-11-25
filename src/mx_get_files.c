#include "uls.h"

static t_type *create_type() {
    t_type *type = malloc(sizeof (t_type));

    type->n_d = 0;
    type->n_e = 0;
    type->n_f = 0;
    type->i = 0;

    return type;
}

static void create_entities(t_entity ***files, t_entity ***dirs, t_entity ***errors, t_entity ***data) {
    int a = 0;
    int dir_counter = 0;
    int err_counter = 0;

    for (int i = 0; (*data)[i] != NULL; i++) {
        if ((*data)[i]->error == NULL) {
            if (!IS_DIR((*data)[i]->info_st.st_mode)) {
                a++;
            } else {
                dir_counter++;
            }
        } else {
            err_counter++;
        }
    }
        
    if (a > 0) {
        *files = malloc((a + 1) * sizeof(t_entity *));
    }
    if (dir_counter > 0) {
        *dirs = malloc((dir_counter + 1) * sizeof(t_entity *));
    } 
    if (err_counter > 0) {
        *errors = malloc((err_counter + 1) * sizeof(t_entity *));
    }
}

static void alloc_dir(t_entity **data, t_type *num, t_entity ***files, t_entity ***dirs) {
    if (!IS_DIR((*data)->info_st.st_mode)) {
        (*files)[num->n_f++] = mx_create_new_file_node((*data));
        (*files)[num->n_f] = NULL;
    }
    else {
        (*dirs)[num->n_d++] = mx_create_new_file_node((*data));
        (*dirs)[num->n_d] = NULL;
    }
}

t_entity **mx_get_files(t_entity ***data, t_flag *flag) {
    t_entity **file_arr = NULL;
    t_entity **dir_arr = NULL;
    t_entity **error_arr = NULL;
    t_type *type = create_type();

    create_entities(&file_arr, &dir_arr, &error_arr, data);

    for (; (*data)[type->i] != NULL; type->i++) {
        if ((*data)[type->i]->error == NULL) {
            alloc_dir(&(*data)[type->i], type, &file_arr, &dir_arr);
        } else {
            error_arr[type->n_e++] = mx_create_new_file_node((*data)[type->i]);
            error_arr[type->n_e] = NULL;
        }
    }

    if (type->n_d > 1) {
        flag->has_files = 1;
    }

    mx_delete_entities(data, dir_arr);
    mx_handle_error(&error_arr, flag);
    free(type);
    
    return file_arr;
}
