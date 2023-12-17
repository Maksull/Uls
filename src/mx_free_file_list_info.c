#include "../inc/uls.h"

// Function to free allocated memory for a list of file_info structures
void mx_free_file_list_info(t_list *files_info) {
    t_list *temp = files_info;

    // Iterate through the list and free each file_info structure
    while (temp) {
        t_file_info *file_info = temp->data;
        mx_free_file_info(file_info);
        temp = temp->next;
    }

    mx_clear_list(&files_info); // Clear the list after freeing all file_info structures
}
