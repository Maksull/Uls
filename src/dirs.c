#include "../inc/uls.h"

// Function to check if a name should be ignored based on ignore_type
bool mx_is_ignored(const char *name, t_ignore_type ignore_type) {
    // Check if the name should be ignored based on the ignore_type
    if (ignore_type == HIDDEN && name[0] == '.')
        return true;
    if (ignore_type == DOTS && (mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0))
        return true;
    return false;
}

// Function to obtain directory entries and store file_info structures in a list
bool mx_get_dir_entries(t_list **entries, const char *name, t_configuration *configuration) {
    DIR *dir = opendir(name);
    if (dir == NULL)
        return false; // Return false if directory cannot be opened

    struct dirent *entry = NULL;

    // Iterate through directory entries and collect file information
    while ((entry = readdir(dir)) != NULL)
        if (!mx_is_ignored(entry->d_name, configuration->ignore_type))
            mx_push_back(entries, mx_get_file_info(name, entry->d_name, configuration));

    closedir(dir);
    return true; // Return true on successful retrieval of directory entries
}

// Function to calculate the total number of blocks used by files in the list
blkcnt_t mx_calculate_count_blocks(t_list *files) {
    t_list *temp = files;
    blkcnt_t blocks_count = 0;

    // Calculate the total number of blocks used by files in the list
    while (temp != NULL) {
        t_file_info *file_info = temp->data;
        blocks_count += file_info->stat.st_blocks;
        temp = temp->next;
    }
    return blocks_count; // Return the total count of blocks
}

// Function to sort and print files in a directory
bool mx_sort_print_dir(t_file_info *file_info, t_configuration *configuration, bool must_print_name) {
    // Print directory name if must_print_name is true
    if (must_print_name) {
        mx_printstr(file_info->name);
        mx_printstr(":\n");
    }

    t_list *files_info = NULL;
    bool result_status = mx_get_dir_entries(&files_info, file_info->path, configuration);

    // Print total block count if detailed format is enabled and files_info is not empty
    if (configuration->format == DETAILED && files_info != NULL) {
        mx_printstr("total ");
        mx_printint(mx_calculate_count_blocks(files_info));
        mx_printchar('\n');
    }

    // Handle errors while obtaining directory entries
    if (!result_status) {
        // Print error message and return false if directory entries cannot be obtained
        mx_printerr("uls: ");
        mx_printerr(file_info->name);
        mx_printerr(": ");
        mx_printerr(strerror(errno));
        mx_printerr("\n");
        return false;
    }

    // Sort and print files' information in the directory
    mx_sort_file_list_info(files_info, configuration->sort_type, configuration->sort_reverse);
    mx_print_files_info(files_info, configuration);

    // Handle recursion if enabled in the configuration settings
    if (configuration->use_recursion) {
        t_list *dirs = NULL;
        while (files_info != NULL) {
            t_file_info *entry = files_info->data;
            // Collect directories for recursion (if not ignored)
            if (S_ISDIR(entry->stat.st_mode) && !mx_is_ignored(entry->name, DOTS)) {
                free(entry->name);
                entry->name = mx_strdup(entry->path);
                mx_push_back(&dirs, entry);
            }
            files_info = files_info->next;
        }

        // Print newline if there are directories to be printed
        if (dirs != NULL)
            mx_printchar('\n');

        // Recursively sort and print directories
        if (!mx_sort_print_dirs(dirs, configuration, true))
            result_status = false;
        mx_clear_list(&dirs);
    }

    mx_free_file_list_info(files_info);
    return result_status; // Return the result status of directory printing
}

// Function to sort and print directories
bool mx_sort_print_dirs(t_list *dirs, t_configuration *configuration, bool must_print_names) {
    bool result = true;

    // Sort directories and print each directory
    mx_sort_file_list_info(dirs, configuration->sort_type, configuration->sort_reverse);
    while (dirs != NULL) {
        t_file_info *file_info = dirs->data;
        // Print and sort each directory, updating the result status
        if (!mx_sort_print_dir(file_info, configuration, must_print_names))
            result = false;
        if (dirs->next != NULL)
            mx_printchar('\n');
        dirs = dirs->next;
    }
    return result; // Return the result status of printing directories
}
