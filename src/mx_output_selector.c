#include "../inc/uls.h"

static void print_single_obj(t_object **name_arr, t_flag *flag) {
    for (int a = 0; name_arr[a]; a++) {
        if (flag->G != 1) {
            mx_printstr(name_arr[a]->name);
            mx_printstr("\n");
        } else {
            mx_printstr_G(name_arr[a]);
            mx_printstr("\n");
        }
    }
}

void mx_output_selector(t_object ***name_arr, t_flag *flag, int flag_num) {
    if (**name_arr != NULL) {
        mx_sort_objects(&(*name_arr), flag);

        (flag->l) ? mx_output_l(*name_arr, flag, flag_num) : (void) 0;

        if (flag->C == 1 && !flag->G) {
            mx_output_c(*name_arr);
        }

        if (flag->X == 1 && !flag->G) {
            mx_output_x(*name_arr);
        }

        if (flag->m == 1) {
            mx_output_m(*name_arr, flag);
        }

        if (flag->is_force == 1) {
            print_single_obj(*name_arr, flag);
        }

        if (flag->G == 1 && flag->m != 1 && flag->l != 1 && flag->is_force != 1) {
            mx_output_G(*name_arr, flag);
        }

        if (flag->l != 1 && flag->C != 1 && flag->X != 1 && flag->m != 1 && flag->is_force != 1 && !flag->G && !flag->g && !flag->o && isatty(1)) {
            mx_output_c(*name_arr);
        }

        if (!isatty(1) && flag->C != 1 && flag->X != 1 && flag->m != 1 && flag->l != 1 && flag->g != 1 && flag->o != 1 && flag->is_force != 1) {
            print_single_obj(*name_arr, flag);
        }
    }
}
