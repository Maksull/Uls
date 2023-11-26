#include "../inc/uls.h"

static char *get_type(t_object *obj) {
    mode_t mode = obj->info_st.st_mode;

    switch (mode & S_IFMT) {
        case S_IFDIR:
            return "d";
        case S_IFLNK:
            return "l";
        case S_IFBLK:
            return "b";
        case S_IFCHR:
            return "c";
        case S_IFIFO:
            return "p";
        case S_IFSOCK:
            return "s";
        case S_IFWHT:
            return "w";
        default:
            return "-";
    }
}

static char *get_acl_info(t_object *obj) {
    acl_t buffer;

    if (listxattr(obj->path, NULL, 0, XATTR_NOFOLLOW) > 0) {
        return "@";
    }

    if ((buffer = acl_get_file(obj->path, ACL_TYPE_EXTENDED))) {
        acl_free(buffer);
        return "+";
    }

    return " ";
}

static char check_chmod_symbol(char value) {
    if (value == '-') {
        return value = 'S';
    }
    
    return value = 's';
}

static char parse_result_str(char *value) {
    if (value[9] == '-') {
        return value[9] = 'T';
    }
    
    return value[9] = 't';
}

void mx_print_permissions(t_object *obj) {
    char *result = mx_strnew(12);

    mx_strcat(result, get_type(obj));

    // user
    mx_strcat(result, (S_IRUSR & obj->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWUSR & obj->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXUSR & obj->info_st.st_mode) ? "x" : "-");

    // group
    mx_strcat(result, (S_IRGRP & obj->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWGRP & obj->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXGRP & obj->info_st.st_mode) ? "x" : "-");

    // other
    mx_strcat(result, (S_IROTH & obj->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWOTH & obj->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXOTH & obj->info_st.st_mode) ? "x" : "-");

    // acl
    mx_strcat(result, get_acl_info(obj));
    mx_strcat(result, "\0");

    S_ISUID & obj->info_st.st_mode ? result[3] = check_chmod_symbol(result[3]) : 0;
    S_ISGID & obj->info_st.st_mode ? result[6] = check_chmod_symbol(result[6]) : 0;
    S_ISVTX & obj->info_st.st_mode ? result[9] = parse_result_str(result) : 0;
    
    mx_printstr(result);

    mx_printchar(' ');
}
