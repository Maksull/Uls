#include "../inc/uls.h"

// Helper function: gets the user name or numeric ID
static char *get_user(uid_t uid, bool numeric_ID) {
    struct passwd *passwd = getpwuid(uid); // Retrieve user information based on the UID

    if (!passwd || numeric_ID) {
        return mx_lltoa(uid); // Return UID as a string if user info not found or numeric ID requested
    }
        
    return mx_strdup(passwd->pw_name); // Otherwise, return the user name
}

// Helper function: gets the group name or numeric ID
static char *get_group(gid_t gid, bool numeric_ID) {
    struct group *group = getgrgid(gid); // Retrieve group information based on the GID
    
    if (!group || numeric_ID) {
        return mx_lltoa(gid); // Return GID as a string if group info not found or numeric ID requested
    }
        
    return mx_strdup(group->gr_name); // Otherwise, return the group name
}

// Helper function: retrieves timespec based on time_type enum
static struct timespec get_timespec(struct stat *stat, t_time_type time_type) {
    switch (time_type) { // Switch to determine which timespec to return based on time_type enum
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

// Function to initialize t_file_info structure
static t_file_info *initialize_file_info(const char *dir, const char *name) {
    t_file_info *file_info = malloc(sizeof(t_file_info));

    // Initialize t_file_info structure members
    file_info->path = mx_delim_strjoin(dir, name, '/'); // Concatenate directory and name with '/'
    file_info->name = mx_strdup(name); // Duplicate the file name
    file_info->user = NULL;
    file_info->link = NULL;
    file_info->group = NULL;
    file_info->xattr_keys = NULL;
    file_info->acl = NULL;

    return file_info;
}

// Function to obtain file statistics
static int obtain_file_stat(t_file_info *file_info, const char *path, t_configuration *configuration) {
    int processing_status;

    if (!path && configuration->follow_symbolic_links) {
        processing_status = stat(file_info->path, &file_info->stat);
    } else {
        processing_status = lstat(file_info->path, &file_info->stat);
    }

    return processing_status;
}

// Function to retrieve detailed information about the file
static void retrieve_detailed_information(t_file_info *file_info, t_configuration *configuration) {
    file_info->user = get_user(file_info->stat.st_uid, configuration->display_numeric_IDs);
    file_info->group = get_group(file_info->stat.st_gid, configuration->display_numeric_IDs);

    if (S_ISLNK(file_info->stat.st_mode)) {
        // Allocate memory for the link and read the link target into it
        file_info->link = malloc(PATH_MAX);
        ssize_t size = readlink(file_info->path, file_info->link, PATH_MAX);
        file_info->link[size] = '\0'; // Null-terminate the link string
    } else {
        file_info->link = NULL;
        // Retrieve ACL information and extended attributes for non-symbolic link files
        file_info->acl = acl_get_file(file_info->path, ACL_TYPE_EXTENDED);

        char buffer[1024] = {'\0'};
        ssize_t size = listxattr(file_info->path, buffer, sizeof(buffer), XATTR_NOFOLLOW);
        for (int i = 0; i < size - 1; i++) {
            if (buffer[i] == '\0') {
                buffer[i] = ':'; // Replace null terminators with colons to split attributes
            }
        }
        if (size > 0) {
            file_info->xattr_keys = mx_strsplit(buffer, ':'); // Split attributes into an array
        } else {
            file_info->xattr_keys = NULL;
        }
    }

    // Get the appropriate timespec based on the configuration time type
    file_info->timespec = get_timespec(&file_info->stat, configuration->time_type);
}

// Function to free memory for failed processing
static void free_memory_for_failed_processing(t_file_info *file_info) {
    mx_free_file_info(file_info); // Free allocated memory for t_file_info structure
}

// Function to obtain file information based on path, name, and configuration settings
t_file_info *mx_get_file_info(const char *dir, const char *name, t_configuration *configuration) {
    t_file_info *file_info = initialize_file_info(dir, name); // Initialize t_file_info structure

    int processing_status = obtain_file_stat(file_info, dir, configuration); // Obtain file statistics

    if (processing_status != 0) {
        free_memory_for_failed_processing(file_info); // Free memory for failed processing
        return NULL; // Return NULL if file statistics couldn't be obtained
    }

    if (configuration->format == DETAILED) {
        retrieve_detailed_information(file_info, configuration); // Retrieve detailed information if format is detailed
    }

    return file_info; // Return file information
}
