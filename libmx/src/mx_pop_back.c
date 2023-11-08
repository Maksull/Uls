#include "libmx.h"

void mx_pop_back(t_list **head) {
    t_list *current;

    if (head == NULL || *head == NULL)
    {
        return;
    }

    current = *head;

    if (current->next == NULL) {
        free(current);
        *head = NULL;
        return;
    }

    while (current->next->next != NULL)
    {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;
}
