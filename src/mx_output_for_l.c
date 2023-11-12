#include "../inc/uls.h"

// Function to print the linked file information
static void print_linked_file(const char* name) 
{
    mx_printstr(" -> ");
    mx_printstr(name);
}

// Function to print a string followed by a single space
static void print_str_single_space(char* str) 
{
    mx_printstr(str);
    mx_printstr(" ");
}

// Function to print a string followed by double spaces
static void print_str_double_space(char* str) 
{
    mx_printstr(str);
    mx_printstr("  ");
}

// Function to output detailed information for a single file in long format
static void output_file_l(t_file* file, t_uls_flags* flags) 
{
    mx_printchar(file->type); // Output file type indicator
    print_str_single_space(file->perms); // Output file permissions
    print_str_single_space(file->num_links); // Output number of hard links
    print_str_double_space(file->user_name); // Output user name
    print_str_double_space(file->group_name); // Output group name
    flags->h ? mx_printchar(' ') : (void) 0; // Output space (optional, for alignment)
    print_str_single_space(file->size); // Output file size
    print_str_single_space(file->lm_month); // Output last modification month
    print_str_single_space(file->lm_day); // Output last modification day
    print_str_single_space(file->lm_time); // Output last modification time

    // Output file name with optional color (if enabled)
    flags->G && isatty(1) ? mx_print_color_file_name(file->name, file->stat.st_mode) : mx_printstr(file->name);
    flags->F ? mx_output_for_F(file->stat.st_mode) : (void) 0; // Output special file indicator
    file->linked_file ? print_linked_file(file->linked_file) : (void) 0; // Output linked file information

    mx_printstr("\n"); // Move to the next line

    // Output extended attributes (if enabled) and ACL information (if available)
    if (flags->at && flags->l && listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0) {
        mx_output_file_xattr(file->path, flags->h);
    }
    if (flags->e && file->acl_str) {
        mx_output_acl_info(file->acl_str);
    }
}

// Function to print the total number of blocks for a directory
static void print_total_blocks(t_file** files) 
{
    mx_printstr("total ");
    mx_printint(mx_get_total_blocks(files));
    mx_printstr("\n");
}

// Function to output detailed information for all files in long format
void mx_output_files_l(t_file** files, bool is_dir, t_uls_flags* flags) 
{
    is_dir && *files ? print_total_blocks(files) : (void) 0; // Print total blocks for a directory

    t_file* file = *files;
    while (file) {
        output_file_l(file, flags); // Output detailed information for each file
        file = file->next;
    }
}
