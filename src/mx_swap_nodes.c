#include "../inc/uls.h"

// Swap the positions of two nodes in a linked list
t_file* mx_swap_nodes(t_file* node1, t_file* node2) {
    t_file* tmp = node2->next;
    node2->next = node1;
    node1->next = tmp;

    // Return the new head of the modified list (which is the second node)
    return node2;
}
