#include "../inc/uls.h"

// Resets the fields of the object info structure to zero
static void reset_obj_info(t_obj_info *info) {
    info->user = 0;
    info->group = 0;
    info->link = 0;
    info->size = 0;
}

// Validates and retrieves the password/user associated with the object
static char *validate_password(t_object *total) {
    struct passwd *password = NULL;
    char *name = NULL;

    password = getpwuid(total->info_st.st_uid);
    if (password) {
        // Found valid password, retrieve and return
        name = mx_strdup(password->pw_name);
        return name;
    } else {
        // If no password found, use user ID instead
        name = mx_itoa(total->info_st.st_uid);
        return name;    
    }
}

// Validates and retrieves the group associated with the object
static char *validate_group(t_object *total) {
    struct group *group = getgrgid(total->info_st.st_gid);
    char *name = NULL;
    
    if (group) {
        // Found valid group, retrieve and return
        name = mx_strdup(group->gr_name);
        return name;
    } else {
        // If no group found, use group ID instead
        name = mx_itoa(total->info_st.st_gid);
        return name;
    }
}

// Initializes the object info structure with object-specific data
static void obj_info_init(t_obj_info *info, t_object *total) {
    // Retrieve user and group names or IDs
    char *name_password = validate_password(total);
    char *name_group = validate_group(total);

    // Update info structure with object-specific data
    if (info->link < total->info_st.st_nlink) {
        info->link = total->info_st.st_nlink; // Update link count if greater
    }

    if (info->size < total->info_st.st_size) {
        info->size = total->info_st.st_size; // Update size if greater
    }

    // Update maximum user/group name lengths for formatting purposes
    if (info->group < mx_strlen(name_group)) {
        info->group = mx_strlen(name_group);
    }

    if (info->user < mx_strlen(name_password)) {
        info->user = mx_strlen(name_password);
    }

    // Free allocated memory for user and group names
    free(name_password);
    free(name_group);
}

// Validates if any object in the array is a device (block or character)
static bool validate_dev(t_object **name_arr, t_obj_info *size) {
    // Check if any object in the array is a block or character device
    for (int i = 0; name_arr[i]; i++) {
        if (IS_BLK(name_arr[i]->info_st.st_mode) || IS_CHR(name_arr[i]->info_st.st_mode)) {
            return size->is_develop = true; // Set flag if device found
        }
    }

    return size->is_develop = false; // No device found
}

// Generates the long listing output
void mx_output_l(t_object **name_arr, t_flag *flag, int flag_num) {
    // Allocate memory for current object info
    t_obj_info *current_info = malloc(sizeof(t_obj_info));
    int block_size_total = 0; // Total block size counter
    int a = 0; // Iterator variable

    // Reset object info structure fields to zero
    reset_obj_info(current_info);

    // Loop through each object and gather specific information for formatting
    for (a = 0; name_arr[a]; a++) {
        block_size_total += name_arr[a]->info_st.st_blocks; // Update total block size
        obj_info_init(current_info, name_arr[a]); // Initialize object info
    }

    // If a flag is set, print the total block count
    if (flag_num == 1) {
        mx_printstr("total ");
        mx_printint(block_size_total);
        mx_printchar('\n');
    }

    // Check if any of the objects in the array are block/character devices
    validate_dev(name_arr, current_info);

    // Loop through each object and generate the long listing output
    for (a = 0; name_arr[a]; a++) {
        mx_print_all(name_arr[a], current_info, flag);
    }

    // Free allocated memory for object info structure
    free(current_info);
}
