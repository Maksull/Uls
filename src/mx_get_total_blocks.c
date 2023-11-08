#include "../inc/uls.h"

int mx_get_total_blocks(t_file** files) 
{
    int total = 0;
    t_file* file = *files;
    
    while (file) {
        total += file->stat.st_blocks;
        file = file->next;
    }

    return total;
}
