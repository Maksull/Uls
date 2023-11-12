#include "../inc/uls.h"

// Function to handle an array of file information, sorting and printing based on flags
void mx_handle_file_array(t_file** files, const char* dir_name, bool is_dir, bool is_single, t_uls_flags* flags) 
{
    // Initialize the sorting algorithm based on specified flags
    mx_sort_init(files, flags);

    // Print the directory name if not a single directory and a directory name is provided
    (!is_single && dir_name) ? mx_print_dir_name(dir_name) : (void) 0;

    // Check flags and call appropriate output function
    if (flags->l) {
        // If long format is specified, reset file parameters length and output in long format
        mx_reset_file_params_length(files, flags);
        mx_output_files_l(files, is_dir, flags);
    } else if (flags->m) {
        // If comma-separated format is specified, output in that format
        mx_output_for_m(files, flags);
    } else {
        // Default case: output in default format
        mx_output_default(files, flags);
    }
}
