#include "../inc/uls.h"

static const char *TAB = "       ";

static void print_spaces(int size) {
    for(int i = 0; i <= size; i++)
        mx_printchar(' ');
}

static void handle_dev(t_object *obj, t_obj_info *info) {
    char *minor_number = mx_get_device_ID_minor_number(obj);
    char *major_number = mx_get_device_ID_major_number(obj); 
    
    if (info->is_develop == true) {
        if (IS_BLK(obj->info_st.st_mode) || IS_CHR(obj->info_st.st_mode)) {
            print_spaces(2 - mx_strlen(major_number));
            mx_printstr(major_number);
            mx_printstr(",");
            mx_printstr(" ");
            print_spaces(2 - mx_strlen(minor_number));
            mx_printstr(minor_number); 
        } else {
            mx_printstr(TAB);
            mx_printint(obj->info_st.st_size);
        }
    } else {
       mx_printint(obj->info_st.st_size); 
    }

    free(major_number);
    free(minor_number);
}

void mx_print_obj_info(t_object *obj, t_obj_info *info) {
    char *res_size = mx_itoa(info->size);
    char *result_now = mx_itoa(obj->info_st.st_size);
    int count = 0;

    if (mx_strlen(result_now) == mx_strlen(res_size)) {
        handle_dev(obj, info);
    } else if (mx_strlen(result_now) < mx_strlen(res_size)) {
        count = mx_strlen(result_now);

        while (count != mx_strlen(res_size)) {
            mx_printstr(" ");
            count++;
        }

        handle_dev(obj, info);
    }
    
    mx_printstr(" ");

    free(result_now);
    free(res_size);
}
