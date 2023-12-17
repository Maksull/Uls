#include "../inc/uls.h"

// Helper function: gets the user name or numeric ID
static char *get_user(uid_t uid, bool numeric_ID) {
    // Retrieve user information based on the UID
    struct passwd *passwd = getpwuid(uid);

    // Check if user info is not found or numeric ID is requested
    if (!passwd || numeric_ID)
    {
        return mx_lltoa(uid); // Return UID as a string
    }
        
    return mx_strdup(passwd->pw_name); // Otherwise, return the user name
}

// Helper function: gets the group name or numeric ID
static char *get_group(gid_t gid, bool numeric_ID) {
    // Retrieve group information based on the GID
    struct group *group = getgrgid(gid);
    
    // Check if group info is not found or numeric ID is requested
    if (!group || numeric_ID)
    {
        return mx_lltoa(gid); // Return GID as a string
    }
        
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

static t_file_info *initialize_file_info(const char *dir, const char *name) {
    t_file_info *file_info = malloc(sizeof(t_file_info));

    file_info->path = mx_delim_strjoin(dir, name, '/');
    file_info->name = mx_strdup(name);
    file_info->user = NULL;
    file_info->link = NULL;
    file_info->group = NULL;
    file_info->xattr_keys = NULL;
    file_info->acl = NULL;

    return file_info;
}

static int obtain_file_stat(t_file_info *file_info, const char *path, t_configuration *configuration) {
    int processing_status;

    if (!path && configuration->follow_symbolic_links) {
        processing_status = stat(file_info->path, &file_info->stat);
    } else {
        processing_status = lstat(file_info->path, &file_info->stat);
    }

    return processing_status;
}

static void retrieve_detailed_information(t_file_info *file_info, t_configuration *configuration) {
    file_info->user = get_user(file_info->stat.st_uid, configuration->display_numeric_IDs);
    file_info->group = get_group(file_info->stat.st_gid, configuration->display_numeric_IDs);

    if (S_ISLNK(file_info->stat.st_mode)) {
        file_info->link = malloc(PATH_MAX);
        ssize_t size = readlink(file_info->path, file_info->link, PATH_MAX);
        file_info->link[size] = '\0';
    } else {
        file_info->link = NULL;
        file_info->acl = acl_get_file(file_info->path, ACL_TYPE_EXTENDED);

        char buffer[1024] = {'\0'};
        ssize_t size = listxattr(file_info->path, buffer, sizeof(buffer), XATTR_NOFOLLOW);
        for (int i = 0; i < size - 1; i++) {
            if (buffer[i] == '\0') {
                buffer[i] = ':';
            }
        }
        if (size > 0) {
            file_info->xattr_keys = mx_strsplit(buffer, ':');
        } else {
            file_info->xattr_keys = NULL;
        }
    }

    file_info->timespec = get_timespec(&file_info->stat, configuration->time_type);
}

static void free_memory_for_failed_processing(t_file_info *file_info) {
    mx_free_file_info(file_info);
}

// Function to obtain file information based on path, name, and configuration settings
t_file_info *mx_get_file_info(const char *dir, const char *name, t_configuration *configuration) {
    t_file_info *file_info = initialize_file_info(dir, name);

    int processing_status = obtain_file_stat(file_info, dir, configuration);

    if (processing_status != 0) {
        free_memory_for_failed_processing(file_info);
        return NULL;
    }

    if (configuration->format == DETAILED) {
        retrieve_detailed_information(file_info, configuration);
    }

    return file_info;
}
