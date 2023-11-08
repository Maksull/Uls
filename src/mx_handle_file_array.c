#include "../inc/uls.h"

void mx_handle_file_array(t_file** files, const char* dir_name, bool is_dir, bool is_single, t_flags* flags) 
{
    mx_sort_init(files, flags);
    
    (!is_single && dir_name) ? mx_print_dir_name(dir_name) : (void) 0;
    
    if (flags->l) {
        mx_reset_file_params_length(files, flags);
        mx_output_files_l(files, is_dir, flags);
    } else if (flags->m) {
        mx_output_for_m(files, flags);
    } else {
        mx_output_default(files, flags);
    }
}
