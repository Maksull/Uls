#include "../inc/uls.h"

bool compare_by_name(t_file* first, t_file* second) 
{
    return (mx_strcmp(first->name, second->name) > 0);
}

bool compare_by_size(t_file* first, t_file* second) 
{
    if (first->stat.st_size == second->stat.st_size) {
        return compare_by_name(first, second);
    }
    
    return (first->stat.st_size < second->stat.st_size);
}

bool compare_by_mtime(t_file* first, t_file* second) 
{
    if (first->stat.st_mtime == second->stat.st_mtime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_mtime < second->stat.st_mtime);
}

bool compare_by_atime(t_file* first, t_file* second) 
{
    if (first->stat.st_atime == second->stat.st_atime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_atime < second->stat.st_atime);    
}

bool compare_by_ctime(t_file* first, t_file* second) 
{
    if (first->stat.st_ctime == second->stat.st_ctime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_ctime < second->stat.st_ctime);    
}

void mx_sort_init(t_file** files, t_flags* flags) 
{
    if (flags->f) return;

    if (flags->S)
    {
        mx_sort_list(files, compare_by_size);
    }
    else if (flags->t && !flags->u && !flags->c)
    {
        mx_sort_list(files, compare_by_mtime);
    }
    else if (flags->u && flags->t && !flags->S)
    {
        mx_sort_list(files, compare_by_atime);
    }
    else if (flags->c && flags->t && !flags->S)
    {
        mx_sort_list(files, compare_by_ctime);
    }
    else if (!flags->t && !flags->S)
    {
        mx_sort_list(files, compare_by_name);
    }
       
    if (flags->r)
    {
        mx_reverse_list(files);
    }
}