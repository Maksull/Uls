#include "../inc/uls.h"

// Function to round a floating-point number to a given precision
static float round(float num, int precision) {
    float base = 1;

    // Multiply the base by 10 according to the precision required
    for (int i = 0; i < precision; i++) {
        base *= 10;
    }
    
    num *= base;

    // Rounding to the nearest integer value
    if (num < 0.0)
        return ((int)(num - 0.5)) / base; // Adjusted downward
    else
        return ((int)(num + 0.5) ) / base; // Adjusted upward
}

// Function to format the size string with appropriate units
static char* format_size_str(float d, int precision, char* letter) {
    char *file_size_str;
    float tmp = d;

    // Check if the value is greater than 10 to round to an integer
    if ((int)tmp / 10 > 0) {
        tmp = round(tmp, 0); // Round to an integer if value is greater than 10
        file_size_str = mx_itoa(tmp); // Convert integer to string
    } else {
        // Round the value to the given precision
        tmp = round(tmp, precision);
        file_size_str = mx_dtoa(tmp, precision); // Convert float to string
    }

    // Concatenate the unit letter to the file size string
    mx_strcat(file_size_str, letter);

    return file_size_str; // Return formatted file size string
}

// Function to get a human-readable file size string from a given size in bytes
char* mx_get_file_size(long int f_size) {
    // Define constants for byte sizes in different units
    int Kbyte = 1024;
    int Mbyte = 1024 * 1024;
    int Gbyte = 1024 * 1024 * 1024;
    long Tbyte = 1024 * 1024 * (long)1024 * 1024;
    long Pbyte = 1024 * 1024 * (long)1024 * 1024 * 1024;

    long int file_size = f_size;
    char *file_size_str;
    float tmp;

    // Determine the appropriate unit for the file size and format accordingly
    if (file_size >= Pbyte) {
        tmp = (float)file_size / Pbyte;
        file_size_str = format_size_str(tmp, 1, "P");
    } else if (file_size >= Tbyte) {
        tmp = (float)file_size / Tbyte;
        file_size_str = format_size_str(tmp, 1, "T");
    } else if (file_size >= Gbyte) {
        tmp = (float)file_size / Gbyte;
        file_size_str = format_size_str(tmp, 1, "G");
    } else if (file_size >= Mbyte) {
        tmp = (float)file_size / Mbyte;
        file_size_str = format_size_str(tmp, 1, "M");
    } else if (file_size >= 1000) {
        tmp = (float)file_size / Kbyte;
        file_size_str = format_size_str(tmp, 1, "K");
    } else {
        // For sizes less than 1000 bytes, display in bytes
        file_size_str = mx_itoa(file_size);
        mx_strcat(file_size_str, "B");
    }
    
    return mx_strdup(file_size_str); // Return the final formatted size string
}
