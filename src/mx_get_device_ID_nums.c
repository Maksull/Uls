#include "../inc/uls.h"

// Converts the minor number to a hexadecimal representation
static char *minor_number_to_hex(char *minor) {
    char *result = mx_strdup("0x00000000");

    int padding = 10 - mx_strlen(minor); // Calculate padding to ensure correct placement

    mx_strcpy(result + padding, minor); // Copy the minor number into the hexadecimal representation
    mx_strdel(&minor); // Free memory allocated for the original minor number

    return result;
}

// Retrieves the minor number of the device ID for the specified object
char *mx_get_device_ID_minor_number(t_object *obj) {
    static const int MAX_MINOR_NUMBER = 255; // Maximum minor number

    char *result = NULL;
    
    int minor_number = (int)(obj->info_st.st_rdev & 0xffffff); // Extract minor number from st_rdev

    if (minor_number > MAX_MINOR_NUMBER) {
        result = minor_number_to_hex(mx_nbr_to_hex(minor_number)); // Convert to hexadecimal if greater than 255
    }

    result = mx_itoa(minor_number); // Convert minor number to string

    return result; // Return the minor number (in decimal or hexadecimal format)
}

// Retrieves the major number of the device ID for the specified object
char *mx_get_device_ID_major_number(t_object *obj) {
    // Extract the major number by shifting and masking the st_rdev value
    char *result = mx_itoa((int)(((unsigned int)obj->info_st.st_rdev >> 24) & 0xff));

    return result; // Return the major number as a string
}
