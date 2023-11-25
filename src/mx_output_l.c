#include "uls.h"

static void reset_size(t_info *size) {
    size->user = 0;
    size->group = 0;
    size->link = 0;
    size->size = 0;
}

static char *validate_password(t_entity *total) {
    struct passwd *password = NULL;
    char *name_str = NULL;

    password = getpwuid(total->info_st.st_uid);
    if (password) {
        name_str = mx_strdup(password->pw_name);

        return name_str;
    } else {
        name_str = mx_itoa(total->info_st.st_uid);

        return name_str;    
    }
}

static char *validate_group(t_entity *total) {
    struct group *group = getgrgid(total->info_st.st_gid);
    char *name_str = NULL;
    
    if (group) {
        name_str = mx_strdup(group->gr_name);

        return name_str;
    } else {
        name_str = mx_itoa(total->info_st.st_gid);

        return name_str;
    }
}

static void size_counter(t_info *size, t_entity *total) {
    char *name_password = validate_password(total);
    char *name_group = validate_group(total);

    if (size->link < total->info_st.st_nlink) {
        size->link = total->info_st.st_nlink;
    }

    if (size->size < total->info_st.st_size) {
        size->size = total->info_st.st_size;
    }

    if (size->group < mx_strlen(name_group)) {
        size->group = mx_strlen(name_group);
    }

    if (size->user < mx_strlen(name_password)) {
        size->user = mx_strlen(name_password);
    }

    free(name_password);
    free(name_group);
}

static bool validate_dev(t_entity **name_arr, t_info *size) {
    for (int i = 0; name_arr[i]; i++) {
        if (IS_BLK(name_arr[i]->info_st.st_mode) || IS_CHR(name_arr[i]->info_st.st_mode)) {
            return size->is_develop = true;
        }
    }

    return size->is_develop = false;
}

void mx_output_l(t_entity **name_arr, t_flag *flag, int flag_num) {
    t_info *current_size = malloc(sizeof(t_info));
    int block_size_total = 0;
    int a = 0;

    reset_size(current_size);
    for (a = 0; name_arr[a]; a++) {
        block_size_total += name_arr[a]->info_st.st_blocks;
        size_counter(current_size, name_arr[a]);
    }

    if (flag_num == 1) {
        mx_printstr("total ");
        mx_printint(block_size_total);
        mx_printchar('\n');
    }

    validate_dev(name_arr, current_size);

    for (a = 0; name_arr[a]; a++) {
        mx_print_all(name_arr[a], current_size, flag);
    }

    free(current_size);
}
