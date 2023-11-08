#include "libmx.h"

void mx_pop_front(t_list **head) {
    t_list *temp;

    if (head == NULL || *head == NULL)
    {
        return;
    }

    temp = *head;
    *head = temp->next;
    free(temp);
}
