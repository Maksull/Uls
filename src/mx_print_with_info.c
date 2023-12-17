#include "../inc/uls.h"

// Function to print the permissions of a file
static void print_permissions(mode_t mode) {
    // Print user, group, and other permissions based on the file's mode
    // Each character represents read (r), write (w), and execute (x) permissions
    // Additionally, 's' and 't' are used for setuid/setgid and sticky bits, respectively
    mx_printchar((mode & S_IRUSR) ? 'r' : '-');
    mx_printchar((mode & S_IWUSR) ? 'w' : '-');
    if (mode & S_IXUSR)
    {
       mx_printchar((mode & S_ISUID) ? 's' : 'x'); 
    }
    else
    {
        mx_printchar((mode & S_ISUID) ? 'S' : '-');
    }
    mx_printchar((mode & S_IRGRP) ? 'r' : '-');
    mx_printchar((mode & S_IWGRP) ? 'w' : '-');
    if (mode & S_IXGRP)
    {
        mx_printchar((mode & S_ISGID) ? 's' : 'x');
    }
    else
    {
        mx_printchar((mode & S_ISGID) ? 'S' : '-');
    }
    mx_printchar((mode & S_IROTH) ? 'r' : '-');
    mx_printchar((mode & S_IWOTH) ? 'w' : '-');
    if (mode & S_IXOTH)
    {
        mx_printchar((mode & S_ISTXT) ? 't' : 'x');
    }
    else
    {
        mx_printchar((mode & S_ISTXT) ? 'T' : '-');
    }
}

// Function to print the file type based on its mode
static void print_filetype(mode_t mode) {
    // Determine and print the file type based on the file's mode
    // Possible types include regular file, directory, symbolic link, etc.
    switch (mode & S_IFMT) {
        case S_IFBLK:
            mx_printchar('b');
            break;
        case S_IFCHR:
            mx_printchar('c');
            break;
        case S_IFDIR:
            mx_printchar('d');
            break;
        case S_IFIFO:
            mx_printchar('p');
            break;
        case S_IFLNK:
            mx_printchar('l');
            break;
        case S_IFSOCK:
            mx_printchar('s');
            break;
        default:
            mx_printchar('-');
            break;
    }
}

// Function to print date and time information in a formatted manner
static void print_date_and_time(time_t ptime, bool full_time_info) {
    // Retrieve the string representation of the provided time
    char *time_str = ctime(&ptime);

    // Split the time string into an array based on spaces
    char **array = mx_strsplit(time_str, ' ');

    // Get the current time
    time_t now = time(NULL);

    // Calculate the duration of six months in seconds
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;

    // Truncate the year information and format the output based on time conditions
    array[4][4] = '\0';
    if (full_time_info) {
        // Print full time information
        for (int i = 1; array[i] != NULL; i++) {
            mx_print_aligned(array[i], 2, true);
            if (array[i + 1] != NULL)
            {
                mx_printchar(' ');
            }
        }
    } else if (ptime + six_months_sec <= now || ptime >= now + six_months_sec) {
        // Print date and year for dates older than six months or future dates
        mx_printstr(array[1]);
        mx_printchar(' ');
        mx_print_aligned(array[2], 2, true);
        mx_printstr("  ");
        mx_printstr(array[4]);
    } else {
        // Print date, time, and hour:minute for recent dates within six months
        mx_printstr(array[1]);
        mx_printchar(' ');
        mx_print_aligned(array[2], 2, true);
        mx_printchar(' ');
        char **arr_time = mx_strsplit(array[3], ':');
        mx_printstr(arr_time[0]);
        mx_printchar(':');
        mx_printstr(arr_time[1]);
        mx_del_strarr(&arr_time);
    }

    // Free allocated memory for the time string array
    mx_del_strarr(&array);
}

// Function to print extended attributes of a file
static void print_xattrs(t_file_info *file_info, bool human_readable) {
    // Loop through the extended attribute keys and print their values
    for (char **ptr = file_info->xattr_keys; *ptr != NULL; ptr++) {
        mx_printchar('\t');
        mx_printstr(*ptr);
        mx_printchar('\t');

        // Retrieve the size of the extended attribute value
        ssize_t value_size = getxattr(file_info->path, *ptr, NULL, 0, 0, XATTR_NOFOLLOW);

        // Print the value in a human-readable or raw format based on the flag
        if (human_readable)
        {
            mx_print_size(value_size, 5);
        }
        else
        {
            mx_print_number_aligned(value_size, 4);
        }
            
        mx_printstr(" \n");
    }
}

// Function to print Access Control List (ACL) information of a file
static void print_acl(acl_t acl) {
    // Convert ACL to text representation
    char *acl_str = acl_to_text(acl, NULL);

    // Split ACL text into lines
    char **lines = mx_strsplit(acl_str, '\n');

    // Iterate through ACL lines and print each entry
    for (int i = 1; lines[i] != NULL; i++) {
        mx_printchar(' ');
        mx_printint(i - 1);
        mx_printstr(": ");

        // Split each line into parts based on ':'
        char **parts_of_line = mx_strsplit(lines[i], ':');

        // Print different parts of the ACL entry
        mx_printstr(parts_of_line[0]);
        mx_printchar(':');
        mx_printstr(parts_of_line[2]);
        mx_printchar(' ');
        mx_printstr(parts_of_line[4]);
        mx_printchar(' ');
        mx_printstr(parts_of_line[5]);
        mx_printchar('\n');

        // Free memory allocated for parts of the ACL entry
        mx_del_strarr(&parts_of_line);
    }

    // Free memory allocated for ACL string and lines
    mx_del_strarr(&lines);
    free(acl_str);
}

// Function to print detailed file information including permissions, sizes, etc.
static void mx_print_file_info_detailed(t_file_info *file_info, t_width *width, t_configuration *configuration) {
    // Print file type and permissions
    print_filetype(file_info->stat.st_mode);
    print_permissions(file_info->stat.st_mode);

    // Determine if extended attributes or ACLs are present and print corresponding markers
    if (file_info->xattr_keys != NULL)
    {
        mx_printchar('@');
    }
    else if (file_info->acl != NULL)
    {
        mx_printchar('+');
    }
    else
    {
        mx_printchar(' ');
    }

    // Print link count, user, group, size, date/time, and file/directory name
    mx_printchar(' ');
    mx_print_number_aligned(file_info->stat.st_nlink, width->links);
    mx_printchar(' ');

    // Print user and group information if not hidden in configuration
    if (!configuration->hide_owner_information) {
        mx_print_aligned(file_info->user, width->user, false);
        mx_printstr("  ");
    }
    if (!configuration->hide_group_information) {
        mx_print_aligned(file_info->group, width->group, false);
        mx_printstr("  ");
    }

    // Print additional information based on file type and configuration settings
    if (configuration->hide_owner_information && configuration->hide_group_information)
        mx_printstr("  ");
    if (S_ISCHR(file_info->stat.st_mode) || S_ISBLK(file_info->stat.st_mode)) {
        // Print special device information for character and block devices
        char *hex_number = mx_nbr_to_hex(file_info->stat.st_rdev);
        char *str;
        if (file_info->stat.st_rdev == 0)
        {
            str = mx_strdup(hex_number);
        }
        else
        {
            str = mx_strjoin("0x", hex_number);
        }
        mx_print_aligned(str, width->size, true);
        free(str);
        free(hex_number);
    } else if (configuration->display_human_readable_size)
    {
        // Print human-readable size if enabled in configuration
        mx_print_size(file_info->stat.st_size, width->size);
    }
    else
    {
        // Print raw size if human-readable size is not enabled
        mx_print_number_aligned(file_info->stat.st_size, width->size);
    }
        
    // Print date and time information
    mx_printchar(' ');
    print_date_and_time(file_info->timespec.tv_sec, configuration->full_time_info);
    mx_printchar(' ');

    // Print file/directory name and additional information if it's a symbolic link
    mx_print_file_info(file_info, configuration);
    if (file_info->link != NULL) {
        mx_printstr(" -> ");
        mx_printstr(file_info->link);
    }
    mx_printchar('\n');

    // Print extended attributes if enabled in configuration and present for the file
    if (configuration->extended_attributes && file_info->xattr_keys != NULL)
    {
        print_xattrs(file_info, configuration->display_human_readable_size);
    }

    // Print ACL if enabled in configuration and present for the file
    if (configuration->acl && file_info->acl != NULL)
    {
        print_acl(file_info->acl);
    }
}

// Function to initialize width structure
static t_width initialize_width() {
    t_width width;
    width.links = 0;
    width.user = 0;
    width.group = 0;
    width.size = 0;
    return width;
}

// Function to calculate the maximum width of various information fields for formatting
static t_width calculate_max_width(t_list *files_info, t_configuration *configuration) {
    t_width width = initialize_width();

    // Loop through file information to determine maximum widths
    while (files_info != NULL) {
        t_file_info *file_info = files_info->data;

        // Calculate and update maximum widths based on file information
        char *nlinks = mx_lltoa(file_info->stat.st_nlink);
        if (width.links < mx_strlen(nlinks))
        {
            width.links = mx_strlen(nlinks);
        }
            
        free(nlinks);
        if (width.user < mx_strlen(file_info->user))
        {
            width.user = mx_strlen(file_info->user);
        }
            
        if (width.group < mx_strlen(file_info->group))
        {
            width.group = mx_strlen(file_info->group);
        }

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
        if (width.size < size_len)
        {
            width.size = size_len;
        }

        // Move to the next file information in the list
        files_info = files_info->next;
    }

    // Return the calculated maximum widths
    return width;
}

// Function to print detailed information for multiple files
void mx_print_with_info(t_list *files_info, t_configuration *configuration) {
    // Calculate the maximum width of various fields for formatting
    t_width max_width = calculate_max_width(files_info, configuration);

    // Loop through file information and print detailed information for each file
    while (files_info != NULL) {
        mx_print_file_info_detailed(files_info->data, &max_width, configuration);
        files_info = files_info->next;
    }
}
