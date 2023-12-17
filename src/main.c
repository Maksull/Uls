#include "../inc/uls.h"

static t_list *collect_files_and_dirs(int argc, char *argv[]) {
    t_list *given_files_and_dirs = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || given_files_and_dirs) {
            mx_push_back(&given_files_and_dirs, argv[i]);
        }
    }
    
    if (!given_files_and_dirs) {
        mx_push_back(&given_files_and_dirs, ".");
    }

    return given_files_and_dirs;
}

static void process_file_info(t_list *given_files_and_dirs, t_list **files, t_list **dirs, t_configuration *configuration, int *result_status) {
    while (given_files_and_dirs) {
        char *file_or_dir = given_files_and_dirs->data;
        given_files_and_dirs = given_files_and_dirs->next;

        t_file_info *file_info = mx_get_file_info(NULL, file_or_dir, configuration);

        if (!file_info) {
            mx_printerr("uls: ");
            mx_printerr(file_or_dir);
            mx_printerr(": ");
            mx_printerr(strerror(errno));
            mx_printerr("\n");
            *result_status = 1;
            continue;
        }

        if (S_ISDIR(file_info->stat.st_mode)) {
            mx_push_back(dirs, file_info);
        } else {
            mx_push_back(files, file_info);
        }
    }
}


static void print_file_info(t_list *files, t_list *dirs, t_configuration *configuration, bool must_print_names, int *result_status) {
    if (files) {
        mx_sort_file_list_info(files, configuration->sort_type, configuration->sort_reverse);
        mx_print_files_info(files, configuration);
    }

    if (files && dirs) {
        mx_printchar('\n');
    }

    if (!mx_sort_print_dirs(dirs, configuration, must_print_names)) {
        *result_status = 1;
    }
}

int main(int argc, char *argv[]) {
    t_list *given_files_and_dirs = collect_files_and_dirs(argc, argv);
    t_configuration *configuration = mx_parse_configuration(argc, argv);

    t_list *files = NULL;
    t_list *dirs = NULL;
    bool must_print_names = mx_list_size(given_files_and_dirs) > 1;
    int result_status = 0;

    mx_sort_filenames(given_files_and_dirs, configuration->sort_type);

    process_file_info(given_files_and_dirs, &files, &dirs, configuration, &result_status);
    print_file_info(files, dirs, configuration, must_print_names, &result_status);

    mx_free_file_list_info(files);
    mx_free_file_list_info(dirs);
    mx_clear_list(&given_files_and_dirs);
    free(configuration);

    return result_status;
}
