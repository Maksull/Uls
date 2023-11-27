#include "../inc/uls.h"

t_configuration *mx_parse_configuration(int argc, char *argv[]) {
    t_configuration *configuration = malloc(sizeof(t_configuration));
    mx_memset(configuration, 0, sizeof(t_configuration));
    if (isatty(1))
        configuration->format = MULTI_COLUMN;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-')
            break;
        for (char *str = argv[i] + 1; *str != '\0'; str++) {
            switch (*str) {
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
                    mx_printerr("uls: illegal option -- ");
                    write(2, str, 1);
                    write(2, "\n", 1);
                    mx_printerr("usage: uls [-@ACFGHRSTUacefghlmnopqrtux1] [file ...]\n");
                    exit(1);
            }
        }
    }

    if (isatty(1))
        configuration->display_non_printable_characters = true;
    else
        configuration->use_colors = false;

    if (!configuration->follow_symbolic_links)
        configuration->follow_symbolic_links = configuration->format != DETAILED && !configuration->use_colors && !configuration->classify;

    if (configuration->sort_type == UNSORTED)
        configuration->ignore_type = NOT_IGNORED;

    return configuration;
}

int main(int argc, char *argv[]) {
    t_list *given_files_and_dirs = NULL;
    for (int i = 1; i < argc; i++)
        if (argv[i][0] != '-' || given_files_and_dirs != NULL)
            mx_push_back(&given_files_and_dirs, argv[i]);
    if (given_files_and_dirs == NULL)
        mx_push_back(&given_files_and_dirs, ".");

    t_configuration *configuration = mx_parse_configuration(argc, argv);

    t_list *files = NULL;
    t_list *dirs = NULL;
    bool must_print_names = mx_list_size(given_files_and_dirs) > 1;
    int result_status = 0;

    mx_sort_filenames(given_files_and_dirs, configuration->sort_type);
    while (given_files_and_dirs != NULL) {
        char *file_or_dir = given_files_and_dirs->data;
        given_files_and_dirs = given_files_and_dirs->next;

        t_file_info *file_info = mx_get_file_info(NULL, file_or_dir, configuration);
        if (file_info == NULL) {
            mx_printerr("uls: ");
            mx_printerr(file_or_dir);
            mx_printerr(": ");
            mx_printerr(strerror(errno));
            mx_printerr("\n");
            result_status = 1;
            continue;
        }

        if (S_ISDIR(file_info->stat.st_mode))
            mx_push_back(&dirs, file_info);
        else
            mx_push_back(&files, file_info);
    }
    if (files != NULL) {
        mx_sort_file_list_info(files, configuration->sort_type, configuration->sort_reverse);
        mx_print_files_info(files, configuration);
    }
    if (files != NULL && dirs != NULL)
        mx_printchar('\n');
    if (!mx_sort_print_dirs(dirs, configuration, must_print_names))
        result_status = 1;
    mx_free_file_list_info(files);
    mx_free_file_list_info(dirs);

    mx_clear_list(&given_files_and_dirs);
    free(configuration);
    return result_status;
}
