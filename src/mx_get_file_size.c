#include "../inc/uls.h"

// Function to get a human-readable file size string from a given size in bytes
char* mx_get_file_size(long int f_size) 
{
    // Array of size suffixes for human-readable representation
    static const char* size_suffix[] = {"B", "K", "M", "G", "T", "P"};
    // Number of elements in the size_suffix array
    static const int size_suffix_count = sizeof(size_suffix) / sizeof(size_suffix[0]);
    
    // Initialize file size and index for suffix array
    long int file_size = f_size;
    int index = 0;

    // Iterate to find the appropriate size suffix
    while (file_size >= 1024 && index < size_suffix_count - 1) {
        file_size /= 1024;
        index++;
    }

    // Variable to store the file size string
    char* file_size_str;

    // Check if the size is divisible by 10 or it's the first suffix (no decimal places needed)
    if (index == 0 || file_size % 10 == 0) {
        // Convert file size to string without decimal places
        file_size_str = mx_itoa(file_size);
    } else {
        // Convert file size to string with one decimal place
        float tmp = (float)file_size;
        file_size_str = mx_dtoa(tmp, 1);
    }

    // Concatenate the size suffix to the file size string
    mx_strcat(file_size_str, size_suffix[index]);

    // Duplicate the string to ensure the original size is not modified
    return mx_strdup(file_size_str);
}
