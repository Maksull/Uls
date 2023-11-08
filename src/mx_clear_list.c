#include "../inc/uls.h"

static void clear_node(t_file* node) 
{
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

    free(node);
}

void mx_clear_list(t_file **list)
{
    t_file *node = *list;
    t_file *next = NULL;

    while (node != NULL)
    {
        next = node->next;
        clear_node(node);
        node = next;
    }
    
    *list = NULL;
}
