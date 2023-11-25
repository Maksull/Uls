#include "uls.h"

static void print_single(t_entity **name_arr, t_flag *flag) {
    for (int a = 0; name_arr[a]; a++) {
        if (flag->has_G != 1) {
            mx_printstr(name_arr[a]->name_str);
            mx_printstr("\n");
        } else {
            mx_printstr_with_color(name_arr[a]);
            mx_printstr("\n");
        }
    }
}

void mx_output_selector(t_entity ***name_arr, t_flag *flag, int flag_num) {
    if (**name_arr != NULL) {
        mx_sort_content(&(*name_arr), flag);

        (flag->has_l) ? mx_output_l(*name_arr, flag, flag_num) : (void) 0;

        if (flag->has_C == 1 && !flag->has_G) {
            mx_output_c(*name_arr);
        }

        if (flag->has_X == 1 && !flag->has_G) {
            mx_output_x(*name_arr);
        }

        if (flag->has_m == 1) {
            mx_output_m(*name_arr, flag);
        }

        if (flag->is_force == 1) {
            print_single(*name_arr, flag);
        }

        if (flag->has_G == 1 && flag->has_m != 1 && flag->has_l != 1 && flag->is_force != 1) {
            mx_output_with_color(*name_arr, flag);
        }

        if (flag->has_l != 1 && flag->has_C != 1 && flag->has_X != 1 && flag->has_m != 1 && flag->is_force != 1 && !flag->has_G && !flag->has_g && !flag->has_o && isatty(1)) {
            mx_output_c(*name_arr);
        }

        if (!isatty(1) && flag->has_C != 1 && flag->has_X != 1 && flag->has_m != 1 && flag->has_l != 1 && flag->has_g != 1 && flag->has_o != 1 && flag->is_force != 1) {
            print_single(*name_arr, flag);
        }
    }
}
