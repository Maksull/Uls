#include "../inc/uls.h"

void mx_reverse_list(t_file** list) 
{
    t_file* previous = NULL;
    t_file* current = *list;
    t_file* next;
    while (current != NULL) {
        next  = current->next;  
        current->next = previous;   
        previous = current;
        current = next;
    }
    *list = previous;
}
