#include "../inc/uls.h"

static char *get_user(uid_t uid, bool numeric_ID) {
    struct passwd *passwd = getpwuid(uid);
    if (passwd == NULL || numeric_ID)
        return mx_lltoa(uid);
    return mx_strdup(passwd->pw_name);
}

static char *get_group(gid_t gid, bool numeric_ID) {
    struct group *group = getgrgid(gid);
    if (group == NULL || numeric_ID)
        return mx_lltoa(gid);
    return mx_strdup(group->gr_name);
}

static struct timespec get_timespec(struct stat *stat, t_time_type time_type) {
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

void mx_free_file_info(t_file_info *file_info) {
    free(file_info->path);
    free(file_info->name);
    mx_strdel(&file_info->user);
    mx_strdel(&file_info->link);
    mx_strdel(&file_info->group);
    mx_del_strarr(&file_info->xattr_keys);
    acl_free(file_info->acl);
    free(file_info);
}

void mx_free_file_list_info(t_list *files_info) {
    t_list *temp = files_info;

    while (temp != NULL) {
        t_file_info *file_info = temp->data;
        mx_free_file_info(file_info);
        temp = temp->next;
    }

    mx_clear_list(&files_info);
}

t_file_info *mx_get_file_info(const char *dir, const char *name, t_configuration *configuration) {
    t_file_info *file_info = malloc(sizeof(t_file_info));

    file_info->path = mx_delim_strjoin(dir, name, '/');
    file_info->name = mx_strdup(name);
    file_info->user = NULL;
    file_info->link = NULL;
    file_info->group = NULL;
    file_info->xattr_keys = NULL;
    file_info->acl = NULL;

    int processing_status;
    if (dir == NULL && configuration->follow_symbolic_links)
        processing_status = stat(file_info->path, &file_info->stat);
    else
        processing_status = lstat(file_info->path, &file_info->stat);
    if (processing_status != 0) {
        mx_free_file_info(file_info);
        return NULL;
    }

    if (configuration->format == DETAILED) {
        file_info->user = get_user(file_info->stat.st_uid, configuration->display_numeric_IDs);
        file_info->group = get_group(file_info->stat.st_gid, configuration->display_numeric_IDs);

        if (S_ISLNK(file_info->stat.st_mode)) {
            file_info->link = malloc(PATH_MAX);
            ssize_t size = readlink(file_info->path, file_info->link, PATH_MAX);
            file_info->link[size] = '\0';
        } else {
            file_info->link = NULL;
            file_info->acl = acl_get_file(file_info->path, ACL_TYPE_EXTENDED);
        }
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
    file_info->timespec = get_timespec(&file_info->stat, configuration->time_type);
    return file_info;
}
