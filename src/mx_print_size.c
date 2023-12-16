#include "../inc/uls.h"

// Function to round a size value to the nearest integer
static double round_size(double number) {
    // Round the provided number to the nearest integer value
    return (long)(number + 0.5);
}

static int determine_measurement_unit(off_t size) {
    const char *measurement_units[] = {"B", "K", "M", "G", "T", "P"};
    double file_size = size;
    int unit_index = 0;

    while (file_size >= 1000) {
        file_size /= 1024;
        unit_index++;
    }

    return unit_index;
}

static void format_size_value(double rounded_size, int unit_index, char *buf) {
    if (rounded_size >= 10 || unit_index == 0) {
        char *str = mx_lltoa(round_size(file_size));
        mx_strcat(buf, str);
        free(str);
    } else {
        char *str = mx_lltoa(rounded_size);
        mx_strcat(buf, str);
        free(str);
        mx_strcat(buf, ".");
        str = mx_lltoa((long long)(rounded_size * 10) % 10);
        mx_strcat(buf, str);
        free(str);
    }
}

// Function to print the size in a human-readable format or as a raw number
void mx_print_size(off_t size, int width) {
    const char *measurement_units[] = {"B", "K", "M", "G", "T", "P"};
    double file_size = size;

    int unit_index = determine_measurement_unit(size);
    // Calculate the rounded size value
    double rounded_size = round_size(file_size * 10) / 10;

    char buf[5] = {'\0'};

    format_size_value(rounded_size, unit_index, buf);

    mx_strcat(buf, measurement_units[unit_index]);

    mx_print_aligned(buf, width, true);
}
