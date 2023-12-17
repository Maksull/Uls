#include "../inc/uls.h"

// Function to apply color to file names based on their mode
static void apply_color(mode_t mode) {
    switch (mode & S_IFMT) {
        // Color codes for different file types
        case S_IFBLK:
            mx_printstr("\033[34;46m");  // Blue background, Cyan text (Block special)
            break;
        case S_IFCHR:
            mx_printstr("\033[34;43m");  // Blue background, Yellow text (Character special)
            break;
        case S_IFDIR:
            // Directory handling based on permission flags
            if (mode & S_IWOTH) {
                if (mode & S_ISTXT) {
                    mx_printstr("\033[30;42m");  // Black background, Green text (Sticky bit set)
                } else {
                    mx_printstr("\033[30;43m");  // Black background, Yellow text (No sticky bit)
                }
            } else {
                mx_printstr("\033[34m");  // Blue text (Regular directory)
            }
            break;
        case S_IFIFO:
            mx_printstr("\033[33m");  // Yellow text (FIFO/pipe)
            break;
        case S_IFLNK:
            mx_printstr("\033[35m");  // Magenta text (Symbolic link)
            break;
        case S_IFSOCK:
            mx_printstr("\033[32m");  // Green text (Socket)
            break;
        default:
            // Handling executable files or other cases with executable permissions
            if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                if (mode & S_ISUID) {
                    mx_printstr("\033[30;41m");  // Black background, Red text (Set UID)
                } else if (mode & S_ISGID) {
                    mx_printstr("\033[30;46m");  // Black background, Cyan text (Set GID)
                } else {
                    mx_printstr("\033[31m");  // Red text (Executable file)
                }
            }
            break;
    }
}

// Function to add classifiers like '/', '|', '@', etc., to file names based on their mode
static int add_classifier(mode_t mode, bool slash_only) {
    char classifier = '\0';

    switch (mode & S_IFMT) {
        case S_IFDIR:
            classifier = '/';  // Directory
            break;
        case S_IFIFO:
            classifier = '|';  // FIFO/pipe
            break;
        case S_IFLNK:
            classifier = '@';  // Symbolic link
            break;
        case S_IFSOCK:
            classifier = '=';  // Socket
            break;
        case S_IFREG:
            if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
                classifier = '*';  // Executable file
            break;
    }

    // Adding the classifier to the file name if applicable
    if (classifier != '\0') {
        if (slash_only && classifier != '/')
            return 0;
        mx_printchar(classifier);
        return 1;
    }
    return 0;
}

// Function to replace non-printable characters in file names with '?'
static char *replace_non_printable_characters(const char *name) {
    char *temp = mx_strdup(name);
    for (int i = 0; temp[i] != '\0'; i++)
        if (temp[i] >= 0 && temp[i] <= 31)
            temp[i] = '?';
    return temp;
}

// Function to apply color to file names based on their mode
static void apply_color_if_enabled(mode_t file_mode, bool use_colors) {
    if (use_colors) {
        apply_color(file_mode); // Apply color to file names based on their mode
    }
}

// Function to handle printing file names with or without replacing non-printable characters
static void print_file_name(t_file_info *file_info, t_configuration *configuration) {
    if (configuration->display_non_printable_characters) {
        char *str = replace_non_printable_characters(file_info->name); // Replace non-printable characters in file names
        mx_printstr(str); // Print the modified file name
        free(str);
    } else {
        mx_printstr(file_info->name); // Print the file name
    }
}

// Function to reset color settings if enabled
static void reset_color_if_enabled(bool use_colors) {
    if (use_colors) {
        mx_printstr("\033[0m"); // Reset color settings
    }
}

// Function to calculate the length of the file name and add classifiers if configured
static int calculate_length_and_classifiers(t_file_info *file_info, t_configuration *configuration) {
    int length = mx_strlen(file_info->name); // Calculate the length of the file name
    if (configuration->classify || configuration->add_only_slash_to_directories) {
        length += add_classifier(file_info->stat.st_mode, configuration->add_only_slash_to_directories); // Add classifiers to file names
    }

    return length; // Return the total length of the printed file name
}

// Function to print file information (name, colors, classifiers) based on configuration settings
int mx_print_file_info(t_file_info *file_info, t_configuration *configuration) {
    apply_color_if_enabled(file_info->stat.st_mode, configuration->use_colors);
    
    print_file_name(file_info, configuration);
    
    reset_color_if_enabled(configuration->use_colors);
    
    int length = calculate_length_and_classifiers(file_info, configuration);

    return length; // Return the total length of the printed file name
}
