#include "../inc/uls.h"

static void apply_color(mode_t mode) {
    switch (mode & S_IFMT) {
        case S_IFBLK:
            mx_printstr("\033[34;46m");
            break;
        case S_IFCHR:
            mx_printstr("\033[34;43m");
            break;
        case S_IFDIR:
            if (mode & S_IWOTH) {
                if (mode & S_ISTXT) {
                    mx_printstr("\033[30;42m");
                } else {
                    mx_printstr("\033[30;43m");
                }
            } else {
                mx_printstr("\033[34m");
            }
            break;
        case S_IFIFO:
            mx_printstr("\033[33m");
            break;
        case S_IFLNK:
            mx_printstr("\033[35m");
            break;
        case S_IFSOCK:
            mx_printstr("\033[32m");
            break;
        default:
            if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                if (mode & S_ISUID) {
                    mx_printstr("\033[30;41m");
                } else if (mode & S_ISGID) {
                    mx_printstr("\033[30;46m");
                } else {
                    mx_printstr("\033[31m");
                }
            }
            break;
    }
}

static int add_classifier(mode_t mode, bool slash_only) {
    char classifier = '\0';

    switch (mode & S_IFMT) {
        case S_IFDIR:
            classifier = '/';
            break;
        case S_IFIFO:
            classifier = '|';
            break;
        case S_IFLNK:
            classifier = '@';
            break;
        case S_IFSOCK:
            classifier = '=';
            break;
        case S_IFREG:
            if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
                classifier = '*';
            break;
    }

    if (classifier != '\0') {
        if (slash_only && classifier != '/')
            return 0;
        mx_printchar(classifier);
        return 1;
    }
    return 0;
}

static char *replace_non_printable_characters(const char *name) {
    char *temp = mx_strdup(name);
    for (int i = 0; temp[i] != '\0'; i++)
        if (temp[i] >= 0 && temp[i] <= 31)
            temp[i] = '?';
    return temp;
}

int mx_print_file_info(t_file_info *file_info, t_configuration *configuration) {
    if (configuration->use_colors)
        apply_color(file_info->stat.st_mode);
    if (configuration->display_non_printable_characters) {
        char *str = replace_non_printable_characters(file_info->name);
        mx_printstr(str);
        free(str);
    } else
        mx_printstr(file_info->name);
    if (configuration->use_colors)
        mx_printstr("\033[0m");

    int length = mx_strlen(file_info->name);
    if (configuration->classify || configuration->add_only_slash_to_directories)
        length += add_classifier(file_info->stat.st_mode, configuration->add_only_slash_to_directories);
    return length;
}

void mx_print_files_info(t_list *files_info, t_configuration *configuration) {
    switch (configuration->format) {
        case ONE_COLUMN:
            mx_print_one_column(files_info, configuration);
            break;
        case MULTI_COLUMN:
            mx_print_multi_column(files_info, configuration);
            break;
        case STREAM:
            mx_print_stream(files_info, configuration);
            break;
        case DETAILED:
            mx_print_with_info(files_info, configuration);
            break;
        default:
            break;
    }
}
