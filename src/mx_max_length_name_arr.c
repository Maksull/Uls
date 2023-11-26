#include "../inc/uls.h"

// Retrieves the maximum length of file/directory names in the given array
int mx_get_max_length_name(t_object **name_arr) {
    int max_length = 0;     // Stores the maximum length of names
    int temp_length = 0;    // Temporary variable to hold each name's length

    // Iterates through the array of object names to find the maximum length
    for (int a = 0; name_arr[a]; a++) {
        temp_length = mx_strlen(name_arr[a]->name); // Calculate the length of each name

        // Updates the max_length if a longer name is encountered
        if (temp_length > max_length) {
            max_length = temp_length;
        }
    }

    // Adjusts the max_length to align with a tab (8 characters) boundary for formatting
    if (max_length % 8 == 0) {
        max_length += 8; // If already divisible by 8, add another 8 for alignment
    } else {
        max_length = 8 - (max_length % 8) + max_length; // Aligns to the next tab boundary
    }

    return max_length; // Returns the adjusted maximum length of names
}
