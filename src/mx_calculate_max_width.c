#include "../inc/uls.h"

// Function to initialize width structure
static t_width initialize_width() {
    t_width width;
    width.links = 0;
    width.user = 0;
    width.group = 0;
    width.size = 0;
    return width;
}

// Function to calculate the width of file links
static int calculate_links_width(t_file_info *file_info) {
    char *nlinks = mx_lltoa(file_info->stat.st_nlink);
    int width = mx_strlen(nlinks);
    free(nlinks);
    return width;
}

// Function to calculate the width of user names
static int calculate_user_width(const char *user) {
    return mx_strlen(user);
}

// Function to calculate the width of group names
static int calculate_group_width(const char *group) {
    return mx_strlen(group);
}

// Function to calculate the width of file size
static int calculate_size_width(const t_file_info *file_info, const t_configuration *configuration) {
    int size_len = 5;

    if (S_ISCHR(file_info->stat.st_mode) || S_ISBLK(file_info->stat.st_mode)) {
        char *wsize = mx_nbr_to_hex(file_info->stat.st_rdev);
        size_len = mx_strlen(wsize) + 2;
        free(wsize);
    } else if (!configuration->display_human_readable_size) {
        char *wsize = mx_lltoa(file_info->stat.st_size);
        size_len = mx_strlen(wsize);
        free(wsize);
    }

    return size_len;
}

// Function to update the width structure based on file information
static void update_width(t_width *width, t_file_info *file_info, const t_configuration *configuration) {
    int links_width = calculate_links_width(file_info);
    if (width->links < links_width) {
        width->links = links_width;
    }

    int user_width = calculate_user_width(file_info->user);
    if (width->user < user_width) {
        width->user = user_width;
    }

    int group_width = calculate_group_width(file_info->group);
    if (width->group < group_width) {
        width->group = group_width;
    }

    int size_width = calculate_size_width(file_info, configuration);
    if (width->size < size_width) {
        width->size = size_width;
    }
}

// Function to calculate the maximum width of various information fields for formatting
t_width mx_calculate_max_width(t_list *files_info, t_configuration *configuration) {
    t_width width = initialize_width();

    while (files_info) {
        t_file_info *file_info = files_info->data;
        update_width(&width, file_info, configuration);
        files_info = files_info->next;
    }

    return width;
}
