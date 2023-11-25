#include "uls.h"

static char *get_type(t_entity *entity) {
    mode_t mode = entity->info_st.st_mode;

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

static char *get_acl_info(t_entity *entity) {
    acl_t buffer;

    if (listxattr(entity->path_str, NULL, 0, XATTR_NOFOLLOW) > 0) {
        return "@";
    }

    if ((buffer = acl_get_file(entity->path_str, ACL_TYPE_EXTENDED))) {
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

void mx_print_permissions(t_entity *entity) {
    char *result = mx_strnew(12);

    mx_strcat(result, get_type(entity));

    // user
    mx_strcat(result, (S_IRUSR & entity->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWUSR & entity->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXUSR & entity->info_st.st_mode) ? "x" : "-");

    // group
    mx_strcat(result, (S_IRGRP & entity->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWGRP & entity->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXGRP & entity->info_st.st_mode) ? "x" : "-");

    // other
    mx_strcat(result, (S_IROTH & entity->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWOTH & entity->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXOTH & entity->info_st.st_mode) ? "x" : "-");

    // acl
    mx_strcat(result, get_acl_info(entity));
    mx_strcat(result, "\0");

    S_ISUID & entity->info_st.st_mode ? result[3] = check_chmod_symbol(result[3]) : 0;
    S_ISGID & entity->info_st.st_mode ? result[6] = check_chmod_symbol(result[6]) : 0;
    S_ISVTX & entity->info_st.st_mode ? result[9] = parse_result_str(result) : 0;
    
    mx_printstr(result);

    mx_printchar(' ');
}
