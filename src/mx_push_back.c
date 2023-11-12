#include "../inc/uls.h"

// Function to determine the file type based on the mode
char get_file_type(mode_t mode) 
{
    // Switch statement to identify the file type based on the mode
    switch (mode & S_IFMT) {
        // Cases for different file types
        case S_IFBLK: return 'b';
        case S_IFCHR: return 'c';
        case S_IFDIR: return 'd';
        case S_IFIFO: return 'p';
        case S_IFLNK: return 'l';
        case S_IFREG: return '-';
        case S_IFSOCK: return 's';
        // Default case for unknown file type
        default: return '?';
    }
}

// Function to get owner permissions, considering 's' for setuid and setgid
static char get_owner_perms(char old_perm, char new_perm) {
    return (old_perm == '-') ? mx_toupper(new_perm) : new_perm;
}

// Function to get additional permissions indicator (@ or +)
static char get_additional_perms(t_file* file, t_uls_flags* flags) 
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

// Function to get file permissions string
char* get_file_permissions(t_file* file, t_uls_flags* flags) 
{
    // Extracting the file mode
    mode_t mode = file->stat.st_mode;
    const int PERMS_LEN = 10;
    // Creating a string to store the file permissions
    char* file_perms = mx_strnew(PERMS_LEN);
    int i = 0;

    // Filling in the file permissions string based on the mode
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

    // Handling special cases for setuid, setgid, and sticky bit
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

// Function to get the last modified date information for a file
void get_file_lm_date(t_file **file, t_uls_flags* flags) 
{
    // Getting the current time
    time_t cur_time;
    time(&cur_time);

    // Extracting the last modified time of the file
    time_t time = (*file)->stat.st_mtime;
    // Adjusting based on flags (access time, change time)
    if (flags->u) {
        time = (*file)->stat.st_atime;
    }
    if (flags->c) {
        time = (*file)->stat.st_ctime;
    }

    // Converting the time to a string format
    char *time_str = ctime(&time);
    char **lm_date = mx_strsplit(time_str, ' ');

    // Extracting the month, day, and time information
    (*file)->lm_month = mx_strdup(lm_date[1]);
    (*file)->lm_day = mx_strdup(lm_date[2]);

    // Deciding whether to display full time or just hours and minutes
    if (flags->T) {
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

// Function to get the linked file of a symbolic link
char* get_linked_file(t_file* file) 
{
    int res_size = file->stat.st_size + 1;
    char* result = mx_strnew(res_size - 1);

    // Reading the linked file path
    int bytes = readlink(file->path, result, res_size);

    // Handling errors in reading the linked file
    if (bytes == -1) {
        mx_strdel(&result);
        return NULL;
    }
    result[bytes] = '\0';

    return result;
}

// Function to create a file object with detailed information
t_file* create_file_obj(const char* dir_name, const char* name, t_uls_flags* flags) 
{
    // Allocating memory for a new file object
    t_file* file = malloc(sizeof(t_file));
    // Constructing the full path of the file
    file->path = mx_get_file_path(dir_name, name);
    // Copying the name of the file
    file->name = mx_strndup(name, mx_strlen(name));
    
    // Populating file information if l flag is present
    if (lstat(file->path, &(file->stat)) == 0) 
    {
        // Retrieving owner and group information
        t_passwd* passwd = getpwuid(file->stat.st_uid);
        t_group* group = getgrgid(file->stat.st_gid);

        // Populating various details of the file
        file->type = get_file_type(file->stat.st_mode);
        file->perms = get_file_permissions(file, flags);
        file->num_links = mx_itoa(file->stat.st_nlink);
        file->user_name = passwd ? mx_strdup(passwd->pw_name) : mx_itoa(file->stat.st_uid);
        file->group_name = group ? mx_strdup(group->gr_name) : mx_itoa(file->stat.st_gid);
        file->size = flags->h ? mx_get_file_size(file->stat.st_size) : mx_itoa(file->stat.st_size);
        get_file_lm_date(&file, flags);

        // Getting the linked file information for symbolic links
        char* linked_file = get_linked_file(file);
        file->linked_file = linked_file ? mx_strdup(linked_file) : NULL;
        mx_strdel(&linked_file);
    }
    file->next = NULL;

    return file;
}

// Function to create a file object with minimal information
t_file* create_default_file_obj(const char* dir_name, const char* name) 
{
    // Allocating memory for a new file object
    t_file* file = malloc(sizeof(t_file));
    // Constructing the full path of the file
    file->path = mx_get_file_path(dir_name, name);
    // Copying the name of the file
    file->name = mx_strndup(name, mx_strlen(name));
    // Retrieving basic file information
    lstat(file->path, &(file->stat));

    // Initializing numerical attributes to NULL
    for (int i = 0; i < 10; ++i) 
    {
        ((&file->num_links)[i]) = NULL;
    }
    file->next = NULL;

    return file;
}

// Function to add a new file node to the end of the file list
void mx_push_back(t_file **list, const char* dir_name, const char* file_name, t_uls_flags* flags)
{
    // Creating a new file node
    t_file* new_node = (!flags->l) ? create_default_file_obj(dir_name, file_name) : create_file_obj(dir_name, file_name, flags);
    t_file *last_node = *list;

    // Handling the case where the list is empty
    if (*list == NULL)
    {
        *list = new_node;
        return;
    }

    // Finding the last node in the list
    while (last_node->next != NULL)
    {
        last_node = last_node->next;
    }

    // Adding the new node to the end of the list
    last_node->next = new_node;
}
