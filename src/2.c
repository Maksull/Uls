#include "../inc/uls.h"

static double round_size(double number) {
    return (long)(number + 0.5);
}

static int determine_unit_index(double file_size) {
    int unit_index = 0;
    while (file_size >= 1000) {
        file_size /= 1024;
        unit_index++;
    }
    return unit_index;
}

static void format_size_string(double rounded_size, int unit_index, char *buf) {
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

static void append_measurement_unit(int unit_index, char *buf) {
    const char *measurement_units[] = {"B", "K", "M", "G", "T", "P"};
    mx_strcat(buf, measurement_units[unit_index]);
}

static void print_aligned_size(char *buf, int width) {
    mx_print_aligned(buf, width, true);
}

void mx_print_size(off_t size, int width) {
    double file_size = size;
    int unit_index = determine_unit_index(file_size);
    double rounded_size = round_size(file_size * 10) / 10;

    char buf[5] = {'\0'};
    format_size_string(rounded_size, unit_index, buf);
    append_measurement_unit(unit_index, buf);
    print_aligned_size(buf, width);
}
