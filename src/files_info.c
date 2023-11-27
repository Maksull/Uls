#include "../inc/uls.h"

// Helper function: gets the user name or numeric ID
static char *get_user(uid_t uid, bool numeric_ID) {
    // Retrieve user information based on the UID
    struct passwd *passwd = getpwuid(uid);
    // Check if user info is not found or numeric ID is requested
    if (passwd == NULL || numeric_ID)
        return mx_lltoa(uid); // Return UID as a string
    return mx_strdup(passwd->pw_name); // Otherwise, return the user name
}

// Helper function: gets the group name or numeric ID
static char *get_group(gid_t gid, bool numeric_ID) {
    // Retrieve group information based on the GID
    struct group *group = getgrgid(gid);
    // Check if group info is not found or numeric ID is requested
    if (group == NULL || numeric_ID)
        return mx_lltoa(gid); // Return GID as a string
    return mx_strdup(group->gr_name); // Otherwise, return the group name
}

// Helper function: retrieves timespec based on time_type enum
static struct timespec get_timespec(struct stat *stat, t_time_type time_type) {
    // Switch to determine which timespec to return based on time_type enum
    switch (time_type) {
        case LAST_MODIFICATION:
            return stat->st_mtimespec;
        case LAST_METADATA_CHANGE:
            return stat->st_ctimespec;
        case LAST_ACCESS:
            return stat->st_atimespec;
        case LAST_CREATION:
            return stat->st_birthtimespec;
    }
}

// Function to free allocated memory for a single file_info structure
void mx_free_file_info(t_file_info *file_info) {
    // Free allocated memory for various fields in file_info structure
    free(file_info->path);
    free(file_info->name);
    mx_strdel(&file_info->user);
    mx_strdel(&file_info->link);
    mx_strdel(&file_info->group);
    mx_del_strarr(&file_info->xattr_keys);
    acl_free(file_info->acl);
    free(file_info);
}

// Function to free allocated memory for a list of file_info structures
void mx_free_file_list_info(t_list *files_info) {
    t_list *temp = files_info;

    // Iterate through the list and free each file_info structure
    while (temp != NULL) {
        t_file_info *file_info = temp->data;
        mx_free_file_info(file_info);
        temp = temp->next;
    }

    mx_clear_list(&files_info); // Clear the list after freeing all file_info structures
}

// Function to obtain file information based on path, name, and configuration settings
t_file_info *mx_get_file_info(const char *dir, const char *name, t_configuration *configuration) {
    t_file_info *file_info = malloc(sizeof(t_file_info)); // Allocate memory for file_info structure

    // Set initial values for various fields in file_info structure
    file_info->path = mx_delim_strjoin(dir, name, '/'); // Join directory and name to create full path
    file_info->name = mx_strdup(name);
    file_info->user = NULL;
    file_info->link = NULL;
    file_info->group = NULL;
    file_info->xattr_keys = NULL;
    file_info->acl = NULL;

    int processing_status;
    // Obtain file information based on whether to follow symbolic links or not
    if (dir == NULL && configuration->follow_symbolic_links)
        processing_status = stat(file_info->path, &file_info->stat);
    else
        processing_status = lstat(file_info->path, &file_info->stat);
    // Check if there's an error obtaining file information, free memory and return NULL
    if (processing_status != 0) {
        mx_free_file_info(file_info);
        return NULL;
    }

    // Retrieve detailed information if the format is set to DETAILED
    if (configuration->format == DETAILED) {
        // Retrieve user and group information
        file_info->user = get_user(file_info->stat.st_uid, configuration->display_numeric_IDs);
        file_info->group = get_group(file_info->stat.st_gid, configuration->display_numeric_IDs);

        // Retrieve symbolic link information or ACLs if it's not a symbolic link
        if (S_ISLNK(file_info->stat.st_mode)) {
            // If it's a symbolic link, get the link target
            file_info->link = malloc(PATH_MAX);
            ssize_t size = readlink(file_info->path, file_info->link, PATH_MAX);
            file_info->link[size] = '\0';
        } else {
            // If it's not a symbolic link, retrieve ACLs
            file_info->link = NULL;
            file_info->acl = acl_get_file(file_info->path, ACL_TYPE_EXTENDED);
        }

        // Retrieve extended attributes keys
        char buffer[1024] = {'\0'};
        ssize_t size = listxattr(file_info->path, buffer, sizeof(buffer), XATTR_NOFOLLOW);
        for (int i = 0; i < size - 1; i++)
            if (buffer[i] == '\0')
                buffer[i] = ':';
        if (size > 0)
            file_info->xattr_keys = mx_strsplit(buffer, ':');
        else
            file_info->xattr_keys = NULL;
    }

    // Retrieve timespec based on the specified time type
    file_info->timespec = get_timespec(&file_info->stat, configuration->time_type);
    return file_info; // Return the populated file_info structure
}
