#include "../inc/uls.h"

// Function to calculate the total number of blocks used by files in the list
blkcnt_t mx_calculate_count_blocks(t_list *files) {
    t_list *temp = files;
    blkcnt_t blocks_count = 0;

    // Calculate the total number of blocks used by files in the list
    while (temp) {
        t_file_info *file_info = temp->data;
        blocks_count += file_info->stat.st_blocks;
        temp = temp->next;
    }
    
    return blocks_count; // Return the total count of blocks
}
