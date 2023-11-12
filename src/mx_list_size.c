#include "../inc/uls.h"

// Function to calculate the size of a linked list
int mx_list_size(t_file *list)
{
    t_file *node = list;
    int size = 0;

    while (node != NULL)
    {
        node = node->next;
        size++;
    }

    // Return the size of the list
    return size;
}
