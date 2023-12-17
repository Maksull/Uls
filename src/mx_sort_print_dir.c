#include "../inc/uls.h"

static void print_directory_name(t_file_info *file_info, bool must_print_name) {
    if (must_print_name) {
        mx_printstr(file_info->name);
        mx_printstr(":\n");
    }
}

static bool handle_directory_entries(t_list **files_info, t_file_info *file_info, t_configuration *configuration) {
    bool result_status = mx_get_dir_entries(files_info, file_info->path, configuration);
    
    if (configuration->format == DETAILED && *files_info) {
        mx_printstr("total ");
        mx_printint(mx_calculate_count_blocks(*files_info));
        mx_printchar('\n');
    }

    if (!result_status) {
        mx_printerr("uls: ");
        mx_printerr(file_info->name);
        mx_printerr(": ");
        mx_printerr(strerror(errno));
        mx_printerr("\n");
    }

    return result_status;
}

static void handle_recursion(t_list *files_info, t_configuration *configuration) {
    t_list *dirs = NULL;
    while (files_info) {
        t_file_info *entry = files_info->data;
        if (S_ISDIR(entry->stat.st_mode) && !mx_is_ignored(entry->name, DOTS)) {
            free(entry->name);
            entry->name = mx_strdup(entry->path);
            mx_push_back(&dirs, entry);
        }
        files_info = files_info->next;
    }

    if (dirs) {
        mx_printchar('\n');
    }

    if (!mx_sort_print_dirs(dirs, configuration, true)) {
        // Handle if recursion fails
    }
    mx_clear_list(&dirs);
}

// Function to sort and print files in a directory
bool mx_sort_print_dir(t_file_info *file_info, t_configuration *configuration, bool must_print_name) {
    print_directory_name(file_info, must_print_name);

    t_list *files_info = NULL;
    bool result_status = handle_directory_entries(&files_info, file_info, configuration);

    if (!result_status) {
        return false;
    }

    // Sort and print files' information in the directory
    mx_sort_file_list_info(files_info, configuration->sort_type, configuration->sort_reverse);
    mx_print_files_info(files_info, configuration);

    if (configuration->use_recursion) {
        handle_recursion(files_info, configuration);
    }
    mx_free_file_list_info(files_info);

    return result_status;
}
