#include "uls.h"

static const char *GAP = "       ";

static void insert_spaces(int size) {
    for(int i = 0; i <= size; i++)
        mx_printchar(' ');
}

static void handle_dev(t_entity *entity, t_info *size) {
    char *minor_number = mx_get_minor_number(entity);
    char *major_number = mx_get_major_number(entity); 
    
    if (size->is_develop == true) {
        if (IS_BLK(entity->info_st.st_mode) || IS_CHR(entity->info_st.st_mode)) {
            insert_spaces(2 - mx_strlen(major_number));
            mx_printstr(major_number);
            mx_printstr(",");
            mx_printstr(" ");
            insert_spaces(2 - mx_strlen(minor_number));
            mx_printstr(minor_number); 
        } else {
            mx_printstr(GAP);
            mx_printint(entity->info_st.st_size);
        }
    } else {
       mx_printint(entity->info_st.st_size); 
    }

    free(major_number);
    free(minor_number);
}

void mx_print_size(t_entity *entity, t_info *size) {
    char *res_size = mx_itoa(size->size);
    char *result_now = mx_itoa(entity->info_st.st_size);
    int count = 0;

    if (mx_strlen(result_now) == mx_strlen(res_size)) {
        handle_dev(entity, size);
    } else if (mx_strlen(result_now) < mx_strlen(res_size)) {
        count = mx_strlen(result_now);

        while (count != mx_strlen(res_size)) {
            mx_printstr(" ");
            count++;
        }

        handle_dev(entity, size);
    }
    
    mx_printstr(" ");

    free(result_now);
    free(res_size);
}
