#include "../inc/uls.h"

t_file* mx_swap_nodes(t_file* node1, t_file* node2) 
{
    t_file* tmp = node2->next;
    node2->next = node1;
    node1->next = tmp;
    
    return node2;
}
