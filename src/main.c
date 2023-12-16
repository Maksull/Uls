#include "../inc/uls.h"

int main(int argc, char *argv[]) {
    // Initialize list to store given files and directories
    t_list *given_files_and_dirs = NULL;

    // Iterate through command-line arguments to gather files and directories
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-' || given_files_and_dirs != NULL) // Check if the argument is not an option or if we've already collected files/dirs
        {
            mx_push_back(&given_files_and_dirs, argv[i]); // Add the argument to the list
        }
    }
        
    if (given_files_and_dirs == NULL)
    {
        mx_push_back(&given_files_and_dirs, "."); // If no files/dirs given, default to current directory
    }

    // Parse command-line arguments to obtain configuration settings
    t_configuration *configuration = mx_parse_configuration(argc, argv);

    // Initialize lists to store files and directories separately
    t_list *files = NULL;
    t_list *dirs = NULL;

    // Check if multiple names were provided to determine if names should be printed
    bool must_print_names = mx_list_size(given_files_and_dirs) > 1;
    int result_status = 0;

    // Sort the given files and directories based on the specified sorting type
    mx_sort_filenames(given_files_and_dirs, configuration->sort_type);

    // Iterate through the given files and directories to process them
    while (given_files_and_dirs != NULL) {
        char *file_or_dir = given_files_and_dirs->data;
        given_files_and_dirs = given_files_and_dirs->next;

        // Obtain file information for each file or directory
        t_file_info *file_info = mx_get_file_info(NULL, file_or_dir, configuration);

        // Handle cases where file information cannot be obtained
        if (file_info == NULL) {
            // Print error message and update result status
            mx_printerr("uls: ");
            mx_printerr(file_or_dir);
            mx_printerr(": ");
            mx_printerr(strerror(errno));
            mx_printerr("\n");
            result_status = 1;
            continue; // Move to the next file or directory
        }

        // Determine if it's a directory and add to the appropriate list
        if (S_ISDIR(file_info->stat.st_mode))
        {
            mx_push_back(&dirs, file_info);
        }
        else
        {
            mx_push_back(&files, file_info);
        }
    }

    // Sort and print file information if files are present
    if (files != NULL) {
        mx_sort_file_list_info(files, configuration->sort_type, configuration->sort_reverse);
        mx_print_files_info(files, configuration);
    }

    // Print newline if both files and directories are present
    if (files != NULL && dirs != NULL)
    {
        mx_printchar('\n');
    }

    // Print directories' information
    if (!mx_sort_print_dirs(dirs, configuration, must_print_names))
    {
        result_status = 1;
    }

    // Free allocated memory for file and directory lists
    mx_free_file_list_info(files);
    mx_free_file_list_info(dirs);

    // Clear and free the list of given files and directories
    mx_clear_list(&given_files_and_dirs);
    free(configuration);

    // Return the result status
    return result_status;
}
