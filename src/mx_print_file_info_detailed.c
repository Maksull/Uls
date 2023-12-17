#include "../inc/uls.h"

// Function to print detailed file information including permissions, sizes, etc.
void mx_print_file_info_detailed(t_file_info *file_info, t_width *width, t_configuration *configuration) {
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