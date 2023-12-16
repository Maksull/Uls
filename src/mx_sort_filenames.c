#include "../inc/uls.h"

// Function to compare two strings for generic sorting
static bool compare_strings(void *first_data, void *second_data) {
    return mx_strcmp(first_data, second_data) > 0; // Compare strings lexically
}

// Function to sort a list of filenames
void mx_sort_filenames(t_list *filenames, t_sort_type sort_type) {
    if (sort_type != UNSORTED)
    {
        mx_sort_list(filenames, compare_strings, false); // Sort list of filenames
    }
}
