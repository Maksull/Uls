#include "../inc/uls.h"

// Function to compare two file names for sorting by name
static bool compare_by_name(void *first_data, void *second_data) {
    t_file_info *file_info1 = first_data;
    t_file_info *file_info2 = second_data;
    
    return mx_strcmp(file_info1->name, file_info2->name) > 0; // Compare file names lexically
}

// Function to compare two files by size for sorting by size
static bool compare_by_size(void *first_data, void *second_data) {
    t_file_info *file_info1 = first_data;
    t_file_info *file_info2 = second_data;

    // Compare files by size and, if equal, by their names lexically
    if (file_info1->stat.st_size == file_info2->stat.st_size)
    {
        return mx_strcmp(file_info1->name, file_info2->name) > 0;
    }
        
    return file_info1->stat.st_size < file_info2->stat.st_size; // Compare file sizes
}

// Function to compare two files by time for sorting by time
static bool compare_by_time(void *first_data, void *second_data) {
    t_file_info *file_info1 = first_data;
    t_file_info *file_info2 = second_data;

    // Compare files by time and, if equal, by their names lexically
    if (file_info1->timespec.tv_sec == file_info2->timespec.tv_sec) {
        if (file_info1->timespec.tv_nsec == file_info2->timespec.tv_nsec)
        {
            return mx_strcmp(file_info1->name, file_info2->name) > 0;
        }
            
        return file_info1->timespec.tv_nsec < file_info2->timespec.tv_nsec; // Compare file nanoseconds
    }

    return file_info1->timespec.tv_sec < file_info2->timespec.tv_sec; // Compare file seconds
}

// Function to sort a list of file information based on the specified sorting type
void mx_sort_file_list_info(t_list *files_info, t_sort_type sort_type, bool isReversed) {
    switch (sort_type) {
        case BY_NAME:
            mx_sort_list(files_info, compare_by_name, isReversed); // Sort list by name
            break;
        case BY_SIZE:
            mx_sort_list(files_info, compare_by_size, isReversed); // Sort list by size
            break;
        case BY_TIME:
            mx_sort_list(files_info, compare_by_time, isReversed); // Sort list by time
            break;
        default:
            break;
    }
}
