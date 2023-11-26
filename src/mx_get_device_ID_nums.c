#include "../inc/uls.h"

static char *minor_number_to_hex(char *minor) {
    char *result = mx_strdup("0x00000000");

    int padding = 10 - mx_strlen(minor);

    mx_strcpy(result + padding, minor);
    mx_strdel(&minor);

    return result;
}

char *mx_get_device_ID_minor_number(t_object *obj) {
    static const int MAX_MINOR_NUMBER = 255;

    char *result = NULL;
    
    int minor_number = (int)(obj->info_st.st_rdev & 0xffffff);

    if (minor_number > MAX_MINOR_NUMBER) {
        result = minor_number_to_hex(mx_nbr_to_hex(minor_number));
    }

    result = mx_itoa(minor_number);

    return result;
}

char *mx_get_device_ID_major_number(t_object *obj) {
    char *result = mx_itoa((int)(((unsigned int)obj->info_st.st_rdev >> 24) & 0xff));

    return result;
}
