#include "uls.h"

int mx_max_lenngth_name_arr(t_entity **name_arr) {
    int max_length = 0;
    int temp_length = 0;

    for (int a = 0; name_arr[a]; a++) {
        temp_length = mx_strlen(name_arr[a]->name_str);
        if (temp_length > max_length) {
            max_length = temp_length;
        }
    }

    if (max_length % 8 == 0) {
        max_length += 8;
    } else {
        max_length = 8 - (max_length % 8) + max_length;
    }

    return max_length;
}
