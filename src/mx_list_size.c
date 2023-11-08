#include "../inc/uls.h"

int mx_list_size(t_file *list)
{
    t_file *node = list;
    int size = 0;

    while (node != NULL)
    {
        node = node->next;
        size++;
    }

    return size;
}
