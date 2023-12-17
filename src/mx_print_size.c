#include "../inc/uls.h"

// Function to round a size value to the nearest integer
static double round_size(double number) {
    return (long)(number + 0.5); // Rounds the number to the nearest integer
}

// Function to determine the appropriate unit index based on the file size
static int determine_unit_index(double file_size) {
    int unit_index = 0;
    // Determine the appropriate unit index based on the file size
    while (file_size >= 1000) {
        file_size /= 1024; // Convert the size to the next unit
        unit_index++; // Increment the unit index
    }
    return unit_index; // Return the determined unit index
}

// Function to format the size string based on the rounded size and unit index
static void format_size_string(double rounded_size, int unit_index, char *buf, double file_size) {
    if (rounded_size >= 10 || unit_index == 0) {
        char *str = mx_lltoa(round_size(file_size));
        mx_strcat(buf, str); // Concatenate the string representation of the size
        free(str);
    } else {
        char *str = mx_lltoa(rounded_size);
        mx_strcat(buf, str); // Concatenate the string representation of the rounded size
        free(str);
        mx_strcat(buf, "."); // Add a decimal point
        str = mx_lltoa((long long)(rounded_size * 10) % 10);
        mx_strcat(buf, str); // Add the fractional part of the rounded size
        free(str);
    }
}

// Function to append the appropriate measurement unit to the buffer
static void append_measurement_unit(int unit_index, char *buf) {
    const char *measurement_units[] = {"B", "K", "M", "G", "T", "P"};
    mx_strcat(buf, measurement_units[unit_index]); // Append the measurement unit
}

// Function to print the size with alignment
static void print_aligned_size(char *buf, int width) {
    mx_print_aligned(buf, width, true); // Print the size with alignment
}

// Function to print the size in a human-readable format
void mx_print_size(off_t size, int width) {
    double file_size = size;
    int unit_index = determine_unit_index(file_size); // Determine the appropriate unit index
    double rounded_size = round_size(file_size * 10) / 10; // Round the file size

    char buf[5] = {'\0'}; // Initialize a buffer to store the formatted size
    format_size_string(rounded_size, unit_index, buf, file_size); // Format the size string
    append_measurement_unit(unit_index, buf); // Append the measurement unit to the buffer
    print_aligned_size(buf, width); // Print the size with alignment
}
