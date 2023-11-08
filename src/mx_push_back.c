#include "../inc/uls.h"

char get_file_type(mode_t mode) 
{
    switch (mode & S_IFMT) {
        case S_IFBLK:
            return 'b';
        case S_IFCHR:
            return 'c';
        case S_IFDIR:
            return 'd';
        case S_IFIFO:
            return 'p';
        case S_IFLNK:
            return 'l';
        case S_IFREG:
            return '-';
        case S_IFSOCK:
            return 's';
        default:
            return '?';
    }
}

static char get_owner_perms(char old_perm, char new_perm) {

    if (old_perm == '-') {
        return mx_toupper(new_perm);
    } else {
        return new_perm;
    }

}

static char get_additional_perms(t_file* file, t_flags* flags) 
{
    ssize_t buf_len = listxattr(file->path, NULL, 0, XATTR_NOFOLLOW);
    acl_t acl_info = acl_get_file(file->path, ACL_TYPE_EXTENDED);
    flags->e ? file->acl_str = acl_to_text(acl_info, NULL) : NULL;

    if (buf_len > 0) {
        return '@';

    } else if (acl_info != NULL) {

        acl_free(acl_info);
        return '+';
    }

    return ' ';
}

char* get_file_permissions(t_file* file, t_flags* flags) 
{
    mode_t mode = file->stat.st_mode;
    const int PERMS_LEN = 10;
    char* file_perms = mx_strnew(PERMS_LEN);
    int i = 0;

    file_perms[i++] = (mode & S_IRUSR) ? 'r' : '-';
    file_perms[i++] = (mode & S_IWUSR) ? 'w' : '-';
    file_perms[i++] = (mode & S_IXUSR) ? 'x' : '-';
    file_perms[i++] = (mode & S_IRGRP) ? 'r' : '-';
    file_perms[i++] = (mode & S_IWGRP) ? 'w' : '-';
    file_perms[i++] = (mode & S_IXGRP) ? 'x' : '-';
    file_perms[i++] = (mode & S_IROTH) ? 'r' : '-';
    file_perms[i++] = (mode & S_IWOTH) ? 'w' : '-';
    file_perms[i++] = (mode & S_IXOTH) ? 'x' : '-';
    file_perms[i++] = get_additional_perms(file, flags);

    if (S_ISUID & mode) {
        file_perms[2] = get_owner_perms(file_perms[2], 's');
    }
    
    if (S_ISGID & mode) {
        file_perms[5] = get_owner_perms(file_perms[5], 's');
    }
        
    if (S_ISVTX & mode) {
        file_perms[8] = get_owner_perms(file_perms[8], 't');
    }

    return file_perms;
}

static char* format_size_str(float d, int precision, char* letter) 
{
    char *file_size_str;
    float tmp = d;

    if ((int)tmp / 10 > 0) {
        tmp = mx_round(tmp, 0);
        file_size_str = mx_itoa(tmp);

    } else {
        tmp = mx_round(tmp, precision);
        file_size_str = mx_dtoa(tmp, precision);
    }

    mx_strcat(file_size_str, letter);

    return file_size_str;
}

void get_file_lm_date(t_file **file, t_flags* flags) 
{
    time_t cur_time;
    time(&cur_time);

    time_t time = (*file)->stat.st_mtime;
    if (flags->u) {
        time = (*file)->stat.st_atime;
    }

    if (flags->c) {
        time = (*file)->stat.st_ctime;
    }

    char *time_str = ctime(&time);
    char **lm_date = mx_strsplit(time_str, ' ');

    (*file)->lm_month = mx_strdup(lm_date[1]);
    (*file)->lm_day = mx_strdup(lm_date[2]);

    if (flags->T)
    {
        (*file)->lm_time = mx_strndup(&time_str[11], 13);
    } else {
        int test_time = cur_time - time;
        if (test_time > SIX_MON_LEN || (test_time < 0 && test_time > (-1 * SIX_MON_LEN))) {
            (*file)->lm_time = mx_strndup(lm_date[4], 4);
        } else {
            (*file)->lm_time = mx_strndup(lm_date[3], 5);
        }
    }

    mx_del_strarr(&lm_date);
}

char* get_linked_file(t_file* file) 
{
    int res_size = file->stat.st_size + 1;
    char* result = mx_strnew(res_size - 1);

    int bytes = readlink(file->path, result, res_size);

    if (bytes == -1) {
        mx_strdel(&result);
        return NULL;
    }
    result[bytes] = '\0';

    return result;
}

t_file* create_file_obj(const char* dir_name, const char* name, t_flags* flags) 
{
    t_file* file = malloc(sizeof(t_file));
    file->path = mx_get_file_path(dir_name, name);
    file->name = mx_strndup(name, mx_strlen(name));
    
    if (lstat(file->path, &(file->stat)) == 0) 
    {
        t_passwd* passwd = getpwuid(file->stat.st_uid);
        t_group* group = getgrgid(file->stat.st_gid);

        file->type = get_file_type(file->stat.st_mode);
        file->perms = get_file_permissions(file, flags);
        file->num_links = mx_itoa(file->stat.st_nlink);
        file->user_name = passwd ? mx_strdup(passwd->pw_name) : mx_itoa(file->stat.st_uid);
        file->group_name = group ? mx_strdup(group->gr_name) : mx_itoa(file->stat.st_gid);
        file->size = flags->h ? mx_get_file_size(file->stat.st_size) : mx_itoa(file->stat.st_size);
        get_file_lm_date(&file, flags);

        char* linked_file = get_linked_file(file);
        file->linked_file = linked_file ? mx_strdup(linked_file) : NULL;
        mx_strdel(&linked_file);
    }
    file->next = NULL;

    return file;

}

t_file* create_default_file_obj(const char* dir_name, const char* name) 
{
    t_file* file = malloc(sizeof(t_file));
    file->path = mx_get_file_path(dir_name, name);
    file->name = mx_strndup(name, mx_strlen(name));
    lstat(file->path, &(file->stat));

    for (int i = 0; i < 10; ++i) 
    {
        ((&file->num_links)[i]) = NULL;
    }
    file->next = NULL;

    return file;
}

void mx_push_back(t_file **list, const char* dir_name, const char* file_name, t_flags* flags)
{
    t_file* new_node;
    if (!flags->l) {
        new_node = create_default_file_obj(dir_name, file_name);
    }
    else {
        new_node = create_file_obj(dir_name, file_name, flags);
    }
        
    t_file *last_node = *list;

    if (*list == NULL)
    {
        *list = new_node;
        return;
    }

    while (last_node->next != NULL)
    {
        last_node = last_node->next;
    }

    last_node->next = new_node;
}
