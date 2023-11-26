#include "../inc/uls.h"

static t_check_struct *create_check() {
    t_check_struct *check = malloc(sizeof (t_check_struct));

    check->check_dirs = 0;
    check->check_errors = 0;
    check->check_files = 0;
    check->i = 0;

    return check;
}

static void create_objects(t_object ***files, t_object ***dirs, t_object ***errors, t_object ***data) {
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
        *files = malloc((a + 1) * sizeof(t_object *));
    }
    if (dir_counter > 0) {
        *dirs = malloc((dir_counter + 1) * sizeof(t_object *));
    } 
    if (err_counter > 0) {
        *errors = malloc((err_counter + 1) * sizeof(t_object *));
    }
}

static void obj_init(t_object **data, t_check_struct *num, t_object ***files, t_object ***dirs) {
    if (!IS_DIR((*data)->info_st.st_mode)) {
        (*files)[num->check_files++] = mx_create_new_file_node((*data));
        (*files)[num->check_files] = NULL;
    }
    else {
        (*dirs)[num->check_dirs++] = mx_create_new_file_node((*data));
        (*dirs)[num->check_dirs] = NULL;
    }
}

t_object **mx_get_files(t_object ***data, t_flag *flag) {
    t_object **file_arr = NULL;
    t_object **dir_arr = NULL;
    t_object **error_arr = NULL;
    t_check_struct *type = create_check();

    create_objects(&file_arr, &dir_arr, &error_arr, data);

    for (; (*data)[type->i] != NULL; type->i++) {
        if ((*data)[type->i]->error == NULL) {
            obj_init(&(*data)[type->i], type, &file_arr, &dir_arr);
        } else {
            error_arr[type->check_errors++] = mx_create_new_file_node((*data)[type->i]);
            error_arr[type->check_errors] = NULL;
        }
    }

    if (type->check_dirs > 1) {
        flag->files = 1;
    }

    mx_free_entities(data, dir_arr);
    mx_print_invalid_dir(&error_arr, flag);
    free(type);
    
    return file_arr;
}
