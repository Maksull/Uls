#include "../inc/uls.h"

t_configuration *mx_parse_configuration(int argc, char *argv[]) {
    // Allocate memory for the configuration struct
    t_configuration *configuration = malloc(sizeof(t_configuration));
    mx_memset(configuration, 0, sizeof(t_configuration)); // Initialize to default values

    // Check if output is a terminal, set the default format accordingly
    if (isatty(1))
        configuration->format = MULTI_COLUMN;

    // Loop through command-line arguments to parse options
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') // Break loop if it's not an option
            break;
        for (char *str = argv[i] + 1; *str != '\0'; str++) {
            // Switch through each option character and set corresponding configuration flags
            switch (*str) {
                // Cases for various flags and options, setting configuration options accordingly
                case 'R':
                    configuration->use_recursion = true;
                    break;
                case 'A':
                    configuration->ignore_type = DOTS;
                    break;
                case 'a':
                    configuration->ignore_type = NOT_IGNORED;
                    break;
                case 'G':
                    configuration->use_colors = true;
                    break;
                case 'F':
                    configuration->classify = true;
                    configuration->add_only_slash_to_directories = false;
                    break;
                case 'p':
                    configuration->add_only_slash_to_directories = true;
                    configuration->classify = false;
                    break;
                case 'r':
                    configuration->sort_reverse = true;
                    break;
                case 'h':
                    configuration->display_human_readable_size = true;
                    break;
                case 'e':
                    configuration->acl = true;
                    break;
                case 'o':
                    configuration->hide_group_information = true;
                    break;
                case 'T':
                    configuration->full_time_info = true;
                    break;
                case '@':
                    configuration->extended_attributes = true;
                    break;
                case 'g':
                    configuration->hide_owner_information = true;
                    break;
                case 'x':
                    configuration->sort_horizontally = true;
                    break;
                case 'n':
                    configuration->display_numeric_IDs = true;
                    break;
                case 'q':
                    configuration->display_non_printable_characters = true;
                    break;
                case 'H':
                    configuration->follow_symbolic_links = true;
                    break;
                case '1':
                    configuration->format = ONE_COLUMN;
                    break;
                case 'C':
                    configuration->format = MULTI_COLUMN;
                    break;
                case 'c':
                    configuration->time_type = LAST_METADATA_CHANGE;
                    break;
                case 'm':
                    configuration->format = STREAM;
                    break;
                case 'l':
                    configuration->format = DETAILED;
                    break;
                case 'u':
                    configuration->time_type = LAST_ACCESS;
                    break;
                case 'U':
                    configuration->time_type = LAST_CREATION;
                    break;
                case 't':
                    configuration->sort_type = BY_TIME;
                    break;
                case 'S':
                    configuration->sort_type = BY_SIZE;
                    break;
                case 'f':
                    configuration->sort_type = UNSORTED;
                    configuration->ignore_type = NOT_IGNORED;
                    break;
                default:
                    // Handling illegal options, printing usage info and exiting if an unknown option is encountered
                    mx_printerr("uls: illegal option -- ");
                    write(2, str, 1);
                    write(2, "\n", 1);
                    mx_printerr("usage: uls [-@ACFGHRSTUacefghlmnopqrtux1] [file ...]\n");
                    exit(1);
            }
        }
    }

    // Check if output is a terminal to adjust additional display settings
    if (isatty(1))
        configuration->display_non_printable_characters = true;
    else
        configuration->use_colors = false;

    // Adjust follow symbolic links if certain conditions are met
    if (!configuration->follow_symbolic_links)
        configuration->follow_symbolic_links = configuration->format != DETAILED && !configuration->use_colors && !configuration->classify;

    // Adjust ignore type if sorting is unsorted
    if (configuration->sort_type == UNSORTED)
        configuration->ignore_type = NOT_IGNORED;

    // Return the populated configuration struct
    return configuration;
}

int main(int argc, char *argv[]) {
    // Initialize list to store given files and directories
    t_list *given_files_and_dirs = NULL;

    // Iterate through command-line arguments to gather files and directories
    for (int i = 1; i < argc; i++)
        if (argv[i][0] != '-' || given_files_and_dirs != NULL) // Check if the argument is not an option or if we've already collected files/dirs
            mx_push_back(&given_files_and_dirs, argv[i]); // Add the argument to the list
    if (given_files_and_dirs == NULL)
        mx_push_back(&given_files_and_dirs, "."); // If no files/dirs given, default to current directory

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
            mx_push_back(&dirs, file_info);
        else
            mx_push_back(&files, file_info);
    }

    // Sort and print file information if files are present
    if (files != NULL) {
        mx_sort_file_list_info(files, configuration->sort_type, configuration->sort_reverse);
        mx_print_files_info(files, configuration);
    }

    // Print newline if both files and directories are present
    if (files != NULL && dirs != NULL)
        mx_printchar('\n');

    // Print directories' information
    if (!mx_sort_print_dirs(dirs, configuration, must_print_names))
        result_status = 1;

    // Free allocated memory for file and directory lists
    mx_free_file_list_info(files);
    mx_free_file_list_info(dirs);

    // Clear and free the list of given files and directories
    mx_clear_list(&given_files_and_dirs);
    free(configuration);

    // Return the result status
    return result_status;
}
