#include "../inc/uls.h"

// Function to print the directory name if required
static void print_directory_name(t_file_info *file_info, bool must_print_name) {
    if (must_print_name) {
        mx_printstr(file_info->name); // Print directory name
        mx_printstr(":\n"); // Add a newline after printing the directory name
    }
}

// Function to handle retrieving directory entries and printing detailed information
static bool handle_directory_entries(t_list **files_info, t_file_info *file_info, t_configuration *configuration) {
    bool result_status = mx_get_dir_entries(files_info, file_info->path, configuration);
    
    // Print the total block count if detailed format is specified and files are present
    if (configuration->format == DETAILED && *files_info) {
        mx_printstr("total ");
        mx_printint(mx_calculate_count_blocks(*files_info)); // Print total block count
        mx_printchar('\n'); // Add a newline after printing the total block count
    }

    // Print an error message if retrieving directory entries fails
    if (!result_status) {
        mx_printerr("uls: ");
        mx_printerr(file_info->name);
        mx_printerr(": ");
        mx_printerr(strerror(errno)); // Print the error message related to the failure
        mx_printerr("\n"); // Add a newline after printing the error message
    }

    return result_status;
}

// Function to handle recursion for subdirectories within the current directory
static void handle_recursion(t_list *files_info, t_configuration *configuration) {
    t_list *dirs = NULL;

    // Iterate through files_info to find directories for recursion
    while (files_info) {
        t_file_info *entry = files_info->data;
        // Check if the entry is a directory and not ignored
        if (S_ISDIR(entry->stat.st_mode) && !mx_is_ignored(entry->name, DOTS)) {
            free(entry->name);
            entry->name = mx_strdup(entry->path); // Update the name to full path for subdirectory
            mx_push_back(&dirs, entry); // Add the directory entry to the list for recursion
        }
        files_info = files_info->next;
    }

    if (dirs) {
        mx_printchar('\n'); // Print a newline before listing subdirectories
    }

    // Sort and print subdirectories if recursion is enabled
    if (!mx_sort_print_dirs(dirs, configuration, true)) {
        // Handle if recursion fails (this section can be implemented as needed)
    }
    mx_clear_list(&dirs); // Clear the list of directories after handling recursion
}

// Function to sort and print files in a directory
bool mx_sort_print_dir(t_file_info *file_info, t_configuration *configuration, bool must_print_name) {
    print_directory_name(file_info, must_print_name); // Print the directory name if required

    t_list *files_info = NULL;
    bool result_status = handle_directory_entries(&files_info, file_info, configuration);

    if (!result_status) {
        return false; // Return false if handling directory entries fails
    }

    // Sort and print files' information in the directory
    mx_sort_file_list_info(files_info, configuration->sort_type, configuration->sort_reverse);
    mx_print_files_info(files_info, configuration); // Print detailed file information

    // Handle recursion if enabled
    if (configuration->use_recursion) {
        handle_recursion(files_info, configuration); // Recursively handle subdirectories
    }
    mx_free_file_list_info(files_info); // Free memory allocated for file info nodes

    return result_status; // Return the overall status of sorting and printing the directory
}
