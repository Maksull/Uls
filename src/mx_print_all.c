#include "../inc/uls.h"

// Prints the number of links
static void print_link(t_object *link, t_obj_info *info) {
    // Obtain the number of links as strings
    char *link_count = mx_itoa(link->info_st.st_nlink);
    char *max_count = mx_itoa(info->link);

    // Aligns the number of links output
    if (mx_strlen(link_count) < mx_strlen(max_count)) {
        int diff = mx_strlen(max_count) - mx_strlen(link_count);
        for (int i = 0; i < diff; i++)
            mx_printchar(' ');
    }
    // Print the number of links
    mx_printint(link->info_st.st_nlink);
    mx_printchar(' ');
    free(link_count);
    free(max_count);
}

// Obtains and prints the user name, adjusting for alignment
static void get_user_name(t_object *obj, int usr) {
    struct passwd *password = getpwuid(obj->info_st.st_uid);
    char *name = password ? mx_strdup(password->pw_name) : mx_itoa(obj->info_st.st_uid);
    int name_len = mx_strlen(name);

    // Print the user name and adjust for alignment
    mx_printstr(name);
    if (name_len < usr) {
        for (int i = name_len; i < usr; i++) {
            mx_printchar(' ');
        }
    }
    mx_printstr("  ");
    free(name);
}

// Obtains and prints the group name, adjusting for alignment
static void get_group_name(t_object *obj, int group) {
    struct group *grp = getgrgid(obj->info_st.st_gid);
    char *name = grp ? mx_strdup(grp->gr_name) : mx_itoa(obj->info_st.st_gid);
    int name_len = mx_strlen(name);

    // Print the group name and adjust for alignment
    mx_printstr(name);
    if (name_len < group) {
        for (int i = name_len; i < group; i++)
            mx_printchar(' ');
    }
    mx_printstr("  ");
    free(name);
}

// Prints symbolic links, if present
static void mx_print_link(t_object *link) {
    // Read the link and print if available
    ssize_t size = link->info_st.st_size;
    ssize_t buf_size = (size == 0) ? 100 : size + 1;
    char * buf = mx_strnew(buf_size);
    ssize_t nbytes = readlink(link->path, buf, buf_size);

    mx_printstr(" -> ");
    if (nbytes >= 0)
        mx_printstr(buf);
    mx_strdel(&buf);
}

// Prints the time details according to flag conditions
static void print_time(t_object *link, char *time, t_flag *flag) {
    int stamp = 1565913600, num;
    if (flag->T == 1) {
        for (int i = 4; i < time[i]; i++)
           mx_printchar(time[i]);
    } else {
        num = link->info_st.st_mtime <= stamp ? 10 : 16;
        for(int i = 4; i < num; i++)
                mx_printchar(time[i]);
        if (num == 10) {
            mx_printstr("  ");
            for (int j = 20; j < 24; j++)
                mx_printchar(time[j]);
        }
    }
    mx_printstr(" ");
}

// Prints the object name, applying coloring and symbolic link detection
static void print_link_and_color(t_object *link, t_flag *flag) {
    if (flag->G == 1) {
        mx_printstr_G(link);

        if (IS_LNK(link->info_st.st_mode)) {
            mx_print_link(link);
        }

    } else if (IS_LNK(link->info_st.st_mode)) {
        mx_printstr(link->name);
        mx_print_link(link);
    } else {
        mx_printstr(link->name);
    }
}

// Prints all details of the object in long format
void mx_print_all(t_object *obj, t_obj_info *size, t_flag *flag) {
    time_t *atime = &obj->info_st.st_atime;
    time_t *chtime = &obj->info_st.st_ctime;
    time_t *mtime = &obj->info_st.st_mtime;

    // Print permissions
    mx_print_permissions(obj);
    // Print number of links
    print_link(obj, size);
    // Print user and group names if specified by flags
    if ((flag->o == 1 && flag->g == 0) || (flag->l == 1 && flag->g == 0))
        get_user_name(obj, size->user);
    if ((flag->g == 1 && flag->o == 0) || (flag->l == 1 && flag->o == 0))
        get_group_name(obj, size->group);
    // Print object size and timestamp
    mx_print_obj_info(obj, size);
    // Customize time display according to flags
    if (flag->u == 1)
        mtime = atime;
    if (flag->c == 1)
        mtime = chtime;
    print_time(obj, ctime(mtime), flag);
    // Print object name and symbolic link details
    print_link_and_color(obj, flag);
    mx_printchar('\n');
}
