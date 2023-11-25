#include "uls.h"

char *mx_get_major_number(t_entity *entity) {
    char *result = mx_itoa((int)(((unsigned int)entity->info_st.st_rdev >> 24) & 0xff));

    return result;
}
