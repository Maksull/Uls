#include "../inc/uls.h"

// Function to reverse the order of a linked list of files
void mx_reverse_list(t_file** list) 
{
    // Initialize pointers to keep track of the previous, current, and next nodes
    t_file* previous = NULL;
    t_file* current = *list;
    t_file* next;

    // Traverse the list and reverse the direction of links
    while (current != NULL) {
        next  = current->next;      // Save the next node
        current->next = previous;   // Reverse the link

        // Move to the next nodes in the list
        previous = current;
        current = next;
    }

    // Update the head of the list to the new first node (previously the last node)
    *list = previous;
}
