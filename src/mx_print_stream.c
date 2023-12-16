#include "../inc/uls.h"

// Function to print file information in a stream format
void mx_print_stream(t_list *files_info, t_configuration *configuration) {
    while (files_info != NULL) {
        mx_print_file_info(files_info->data, configuration); // Print file information
        if (files_info->next != NULL)
        {
            mx_printstr(", "); // Print a comma and space if there are more files to be printed
        }
            
        files_info = files_info->next; // Move to the next file in the list
    }
    
    mx_printchar('\n'); // Move to a new line after printing all file information
}
