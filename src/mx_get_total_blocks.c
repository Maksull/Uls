#include "../inc/uls.h"

// Function to calculate the total number of blocks used by a list of files
int mx_get_total_blocks(t_file** files) 
{
    // Initialize the total blocks count
    int total = 0;

    // Get the first file in the list
    t_file* file = *files;

    // Loop through the list and accumulate the st_blocks values of each file
    while (file) {
        total += file->stat.st_blocks;
        file = file->next;
    }

    // Return the total blocks count
    return total;
}
