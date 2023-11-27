#include "../inc/uls.h"

static bool compare_strings(void *first_data, void *second_data) {
    return mx_strcmp(first_data, second_data) > 0;
}

static bool compare_by_name(void *first_data, void *second_data) {
    t_file_info *file_info1 = first_data;
    t_file_info *file_info2 = second_data;
    return mx_strcmp(file_info1->name, file_info2->name) > 0;
}

static bool compare_by_size(void *first_data, void *second_data) {
    t_file_info *file_info1 = first_data;
    t_file_info *file_info2 = second_data;
    if (file_info1->stat.st_size == file_info2->stat.st_size)
        return mx_strcmp(file_info1->name, file_info2->name) > 0;
    return file_info1->stat.st_size < file_info2->stat.st_size;
}

static bool compare_by_time(void *first_data, void *second_data) {
    t_file_info *file_info1 = first_data;
    t_file_info *file_info2 = second_data;
    if (file_info1->timespec.tv_sec == file_info2->timespec.tv_sec) {
        if (file_info1->timespec.tv_nsec == file_info2->timespec.tv_nsec)
            return mx_strcmp(file_info1->name, file_info2->name) > 0;
        return file_info1->timespec.tv_nsec < file_info2->timespec.tv_nsec;
    }
    return file_info1->timespec.tv_sec < file_info2->timespec.tv_sec;
}

void mx_sort_filenames(t_list *filenames, t_sort_type sort_type) {
    if (sort_type != UNSORTED)
        mx_sort_list(filenames, compare_strings, false);
}

void mx_sort_file_list_info(t_list *files_info, t_sort_type sort_type, bool isReversed) {
    switch (sort_type) {
        case BY_NAME:
            mx_sort_list(files_info, compare_by_name, isReversed);
            break;
        case BY_SIZE:
            mx_sort_list(files_info, compare_by_size, isReversed);
            break;
        case BY_TIME:
            mx_sort_list(files_info, compare_by_time, isReversed);
            break;
        default:
            break;
    }
}
