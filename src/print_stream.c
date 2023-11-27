#include "../inc/uls.h"

void mx_print_stream(t_list *files_info, t_configuration *configuration) {
    while (files_info != NULL) {
        mx_print_file_info(files_info->data, configuration);
        if (files_info->next != NULL)
            mx_printstr(", ");
        files_info = files_info->next;
    }
    mx_printchar('\n');
}
