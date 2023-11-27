#include "../inc/uls.h"

void mx_print_one_column(t_list *files_info, t_configuration *configuration) {
    while (files_info != NULL) {
        mx_print_file_info(files_info->data, configuration);
        mx_printchar('\n');
        files_info = files_info->next;
    }
}
