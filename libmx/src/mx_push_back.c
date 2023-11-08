#include "libmx.h"

void mx_push_back(t_list **list, void *data) {
    t_list *new_node = (t_list *)malloc(sizeof(t_list));

    if (new_node == NULL) {
        return;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (*list == NULL) {
        *list = new_node;
        return;
    }

    t_list *current = *list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}
