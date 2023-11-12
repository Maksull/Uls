#include "../inc/uls.h"

// Function to clear the memory of a single node in the file list
static void clear_node(t_file* node) 
{
    // Free allocated memory for various attributes of the node
    mx_strdel(&node->path);
    mx_strdel(&node->name);
    mx_strdel(&node->perms);
    mx_strdel(&node->num_links);
    mx_strdel(&node->user_name);
    mx_strdel(&node->group_name);
    mx_strdel(&node->size);
    mx_strdel(&node->lm_month);
    mx_strdel(&node->lm_day);
    mx_strdel(&node->lm_time);
    mx_strdel(&node->linked_file);

    // Free the memory occupied by the node itself
    free(node);
}

// Function to clear the entire file list
void mx_clear_list(t_file **list)
{
    t_file *node = *list;
    t_file *next = NULL;

    // Loop through the list and clear each node
    while (node != NULL)
    {
        next = node->next;
        clear_node(node);
        node = next;
    }
    
    // Set the list pointer to NULL after clearing all nodes
    *list = NULL;
}
