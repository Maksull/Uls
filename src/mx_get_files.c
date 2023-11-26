#include "../inc/uls.h"

// Creates and initializes the check struct for counting types of objects
static t_check_struct *create_check() {
    t_check_struct *check = malloc(sizeof(t_check_struct));

    // Initialize counters and index
    check->check_dirs = 0;
    check->check_errors = 0;
    check->check_files = 0;
    check->i = 0;

    return check;
}

// Creates arrays for files, directories, and errors based on the data provided
static void create_objects(t_object ***files, t_object ***dirs, t_object ***errors, t_object ***data) {
    int file_counter = 0;
    int dir_counter = 0;
    int err_counter = 0;

    // Count the number of files, directories, and errors in the data array
    for (int i = 0; (*data)[i] != NULL; i++) {
        if ((*data)[i]->error == NULL) {
            if (!IS_DIR((*data)[i]->info_st.st_mode)) {
                file_counter++;
            } else {
                dir_counter++;
            }
        } else {
            err_counter++;
        }
    }
        
    // Allocate memory for arrays if there are corresponding objects
    if (file_counter > 0) {
        *files = malloc((file_counter + 1) * sizeof(t_object *));
    }
    if (dir_counter > 0) {
        *dirs = malloc((dir_counter + 1) * sizeof(t_object *));
    } 
    if (err_counter > 0) {
        *errors = malloc((err_counter + 1) * sizeof(t_object *));
    }
}

// Initializes file or directory objects based on their type
static void obj_init(t_object **data, t_check_struct *num, t_object ***files, t_object ***dirs) {
    if (!IS_DIR((*data)->info_st.st_mode)) {
        // Create and add file object to the files array
        (*files)[num->check_files++] = mx_create_new_file_node((*data));
        (*files)[num->check_files] = NULL;
    } else {
        // Create and add directory object to the dirs array
        (*dirs)[num->check_dirs++] = mx_create_new_file_node((*data));
        (*dirs)[num->check_dirs] = NULL;
    }
}

// Retrieves file objects from the data array and handles directories and errors
t_object **mx_get_files(t_object ***data, t_flag *flag) {
    t_object **file_arr = NULL;
    t_object **dir_arr = NULL;
    t_object **error_arr = NULL;
    t_check_struct *type = create_check(); // Initialize counters

    create_objects(&file_arr, &dir_arr, &error_arr, data); // Allocate memory for arrays

    // Iterate through data array to categorize objects
    for (; (*data)[type->i] != NULL; type->i++) {
        if ((*data)[type->i]->error == NULL) {
            obj_init(&(*data)[type->i], type, &file_arr, &dir_arr); // Initialize file or directory objects
        } else {
            error_arr[type->check_errors++] = mx_create_new_file_node((*data)[type->i]); // Handle errors
            error_arr[type->check_errors] = NULL;
        }
    }

    // Check for multiple directories
    if (type->check_dirs > 1) {
        flag->files = 1; // Set flag for multiple directories
    }

    mx_free_entities(data, dir_arr); // Free directory array memory
    mx_print_invalid_dir(&error_arr, flag); // Print error messages
    free(type); // Free memory allocated for counters

    return file_arr; // Return array of file objects
}
