#include "../inc/uls.h"

// Function to compare two files based on their names
bool compare_by_name(t_file* first, t_file* second) 
{
    return (mx_strcmp(first->name, second->name) > 0);
}

// Function to compare two files based on their sizes
bool compare_by_size(t_file* first, t_file* second) 
{
    if (first->stat.st_size == second->stat.st_size) {
        return compare_by_name(first, second);
    }
    
    return (first->stat.st_size < second->stat.st_size);
}

// Function to compare two files based on their modification times
bool compare_by_mtime(t_file* first, t_file* second) 
{
    if (first->stat.st_mtime == second->stat.st_mtime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_mtime < second->stat.st_mtime);
}

// Function to compare two files based on their access times
bool compare_by_atime(t_file* first, t_file* second) 
{
    if (first->stat.st_atime == second->stat.st_atime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_atime < second->stat.st_atime);    
}

// Function to compare two files based on their change times
bool compare_by_ctime(t_file* first, t_file* second) 
{
    if (first->stat.st_ctime == second->stat.st_ctime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_ctime < second->stat.st_ctime);    
}

// Function to initialize the sorting process based on specified flags
void mx_sort_init(t_file** files, t_uls_flags* flags) 
{
    // Skip sorting if the '-f' flag is present
    if (flags->f) return;

    // Choose the appropriate sorting method based on the specified flags
    if (flags->S) {
        mx_sort_list(files, compare_by_size);
    } else if (flags->t && !flags->u && !flags->c) {
        mx_sort_list(files, compare_by_mtime);
    } else if (flags->u && flags->t && !flags->S) {
        mx_sort_list(files, compare_by_atime);
    } else if (flags->c && flags->t && !flags->S) {
        mx_sort_list(files, compare_by_ctime);
    } else if (!flags->t && !flags->S) {
        mx_sort_list(files, compare_by_name);
    }
       
    // Reverse the list if the '-r' flag is present
    if (flags->r) {
        mx_reverse_list(files);
    }
}
