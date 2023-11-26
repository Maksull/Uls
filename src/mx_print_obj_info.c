#include "../inc/uls.h"

// A constant representing a tab space
static const char *TAB = "       ";

// Function to print spaces based on a given size
static void print_spaces(int size) {
    for(int i = 0; i <= size; i++)
        mx_printchar(' ');
}

// Handles the display of device IDs and file sizes
static void handle_dev(t_object *obj, t_obj_info *info) {
    // Get the minor and major numbers for devices
    char *minor_number = mx_get_device_ID_minor_number(obj);
    char *major_number = mx_get_device_ID_major_number(obj); 
    
    // Display device IDs if the object represents a block or character device
    if (info->is_develop == true) {
        if (IS_BLK(obj->info_st.st_mode) || IS_CHR(obj->info_st.st_mode)) {
            print_spaces(2 - mx_strlen(major_number));
            mx_printstr(major_number);
            mx_printstr(",");
            mx_printstr(" ");
            print_spaces(2 - mx_strlen(minor_number));
            mx_printstr(minor_number); 
        } else {
            // Display the size if it's not a device
            mx_printstr(TAB);
            mx_printint(obj->info_st.st_size);
        }
    } else {
        // Display the size for regular files
       mx_printint(obj->info_st.st_size); 
    }

    free(major_number);
    free(minor_number);
}

// Prints the object information (size)
void mx_print_obj_info(t_object *obj, t_obj_info *info) {
    char *res_size = mx_itoa(info->size); // Convert total size to string
    char *result_now = mx_itoa(obj->info_st.st_size); // Current object size as string
    int count = 0;

    // Compare the lengths of current size and total size
    if (mx_strlen(result_now) == mx_strlen(res_size)) {
        // If equal, handle the device or regular file display
        handle_dev(obj, info);
    } else if (mx_strlen(result_now) < mx_strlen(res_size)) {
        // If current size length is less than total size length, align with spaces
        count = mx_strlen(result_now);
        while (count != mx_strlen(res_size)) {
            mx_printstr(" ");
            count++;
        }
        // After aligning, handle the device or regular file display
        handle_dev(obj, info);
    }
    
    mx_printstr(" "); // Print a space after the size display

    free(result_now);
    free(res_size);
}
