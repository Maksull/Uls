#include "../inc/uls.h"

static void reset_obj_info(t_obj_info *info) {
    info->user = 0;
    info->group = 0;
    info->link = 0;
    info->size = 0;
}

static char *validate_password(t_object *total) {
    struct passwd *password = NULL;
    char *name = NULL;

    password = getpwuid(total->info_st.st_uid);
    if (password) {
        name = mx_strdup(password->pw_name);

        return name;
    } else {
        name = mx_itoa(total->info_st.st_uid);

        return name;    
    }
}

static char *validate_group(t_object *total) {
    struct group *group = getgrgid(total->info_st.st_gid);
    char *name = NULL;
    
    if (group) {
        name = mx_strdup(group->gr_name);

        return name;
    } else {
        name = mx_itoa(total->info_st.st_gid);

        return name;
    }
}

static void obj_info_init(t_obj_info *info, t_object *total) {
    char *name_password = validate_password(total);
    char *name_group = validate_group(total);

    if (info->link < total->info_st.st_nlink) {
        info->link = total->info_st.st_nlink;
    }

    if (info->size < total->info_st.st_size) {
        info->size = total->info_st.st_size;
    }

    if (info->group < mx_strlen(name_group)) {
        info->group = mx_strlen(name_group);
    }

    if (info->user < mx_strlen(name_password)) {
        info->user = mx_strlen(name_password);
    }

    free(name_password);
    free(name_group);
}

static bool validate_dev(t_object **name_arr, t_obj_info *size) {
    for (int i = 0; name_arr[i]; i++) {
        if (IS_BLK(name_arr[i]->info_st.st_mode) || IS_CHR(name_arr[i]->info_st.st_mode)) {
            return size->is_develop = true;
        }
    }

    return size->is_develop = false;
}

void mx_output_l(t_object **name_arr, t_flag *flag, int flag_num) {
    t_obj_info *current_info = malloc(sizeof(t_obj_info));
    int block_size_total = 0;
    int a = 0;

    reset_obj_info(current_info);
    for (a = 0; name_arr[a]; a++) {
        block_size_total += name_arr[a]->info_st.st_blocks;
        obj_info_init(current_info, name_arr[a]);
    }

    if (flag_num == 1) {
        mx_printstr("total ");
        mx_printint(block_size_total);
        mx_printchar('\n');
    }

    validate_dev(name_arr, current_info);

    for (a = 0; name_arr[a]; a++) {
        mx_print_all(name_arr[a], current_info, flag);
    }

    free(current_info);
}
