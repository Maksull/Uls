#include "../inc/uls.h";

void mx_sort_list(t_file** list, bool (*cmp)(t_file *a, t_file *b)) 
{
    t_file** file;
    bool is_swapped;
  
    int count = mx_list_size(*list);

    for (int i = 0; i < count; i++) {
        file = list;
        is_swapped = false;
  
        for (int j = 0; j < count - i - 1; j++) {
  
            t_file* f1 = *file;
            t_file* f2 = f1->next;
  
            if (cmp(f1, f2)) {

                *file = mx_swap_nodes(f1, f2);
                is_swapped = true;
            
            }
  
            file = &(*file)->next;
        }
        if (!is_swapped)
        {
            break;
        }
    }
}
