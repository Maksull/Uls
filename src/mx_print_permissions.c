#include "../inc/uls.h"

// Returns a string representing the type of the object (directory, link, etc.)
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

// Returns a string indicating whether the object has ACLs or extended attributes
static char *get_acl_info(t_object *obj) {
    acl_t buffer;

    // Check for extended attributes
    if (listxattr(obj->path, NULL, 0, XATTR_NOFOLLOW) > 0) {
        return "@";
    }

    // Check for ACLs
    if ((buffer = acl_get_file(obj->path, ACL_TYPE_EXTENDED))) {
        acl_free(buffer);
        return "+";
    }

    return " ";
}

// Checks and updates the execute symbol in the permission string based on set-user-ID or set-group-ID
static char check_chmod_symbol(char value) {
    if (value == '-') {
        return value = 'S'; // If execute is not set, change to 'S'
    }
    
    return value = 's'; // If execute is set, change to 's'
}

// Parses the permission string to replace the execute symbol with uppercase for sticky bit
static char parse_result_str(char *value) {
    if (value[9] == '-') {
        return value[9] = 'T'; // If execute is not set, change to 'T'
    }
    
    return value[9] = 't'; // If execute is set, change to 't'
}

// Prints the permission string for an object
void mx_print_permissions(t_object *obj) {
    char *result = mx_strnew(12); // Allocate memory for the permission string

    // Build the permission string
    mx_strcat(result, get_type(obj));

    // User permissions
    mx_strcat(result, (S_IRUSR & obj->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWUSR & obj->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXUSR & obj->info_st.st_mode) ? "x" : "-");

    // Group permissions
    mx_strcat(result, (S_IRGRP & obj->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWGRP & obj->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXGRP & obj->info_st.st_mode) ? "x" : "-");

    // Other permissions
    mx_strcat(result, (S_IROTH & obj->info_st.st_mode) ? "r" : "-");
    mx_strcat(result, (S_IWOTH & obj->info_st.st_mode) ? "w" : "-");
    mx_strcat(result, (S_IXOTH & obj->info_st.st_mode) ? "x" : "-");

    // Append ACL or extended attribute information
    mx_strcat(result, get_acl_info(obj));
    mx_strcat(result, "\0"); // Null-terminate the string

    // Modify permission string based on special modes (set-user-ID, set-group-ID, sticky bit)
    S_ISUID & obj->info_st.st_mode ? result[3] = check_chmod_symbol(result[3]) : 0;
    S_ISGID & obj->info_st.st_mode ? result[6] = check_chmod_symbol(result[6]) : 0;
    S_ISVTX & obj->info_st.st_mode ? result[9] = parse_result_str(result) : 0;
    
    // Print the permission string
    mx_printstr(result);

    mx_printchar(' '); // Print a space after the permission string
}
