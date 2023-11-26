#include "../inc/uls.h"

// Prints a single object based on the specified flag
static void print_single_obj(t_object **name_arr, t_flag *flag) {
    for (int a = 0; name_arr[a]; a++) {
        // Print the object name based on the flag (G or not)
        if (flag->G != 1) {
            mx_printstr(name_arr[a]->name);
            mx_printstr("\n");
        } else {
            mx_printstr_G(name_arr[a]);
            mx_printstr("\n");
        }
    }
}

// Function that selects the appropriate output format based on flags and terminal settings
void mx_output_selector(t_object ***name_arr, t_flag *flag, int flag_num) {
    if (**name_arr != NULL) {
        // Sorts the objects based on the specified flag
        mx_sort_objects(&(*name_arr), flag);

        // Output in long format if '-l' flag is set
        (flag->l) ? mx_output_l(*name_arr, flag, flag_num) : (void) 0;

        // Output in columns format if '-C' flag is set and color is disabled
        if (flag->C == 1 && !flag->G) {
            mx_output_c(*name_arr);
        }

        // Output in horizontal sort order if '-X' flag is set and color is disabled
        if (flag->X == 1 && !flag->G) {
            mx_output_x(*name_arr);
        }

        // Output in comma-separated format if '-m' flag is set
        if (flag->m == 1) {
            mx_output_m(*name_arr, flag);
        }

        // Output single objects directly if '-1' (force output) flag is set
        if (flag->is_force == 1) {
            print_single_obj(*name_arr, flag);
        }

        // Output in 'G' format if '-G' flag is set
        if (flag->G == 1 && flag->m != 1 && flag->l != 1 && flag->is_force != 1) {
            mx_output_G(*name_arr, flag);
        }

        // Output in column format if no flags explicitly specified and the output is a terminal
        if (flag->l != 1 && flag->C != 1 && flag->X != 1 && flag->m != 1 && flag->is_force != 1 && !flag->G && !flag->g && !flag->o && isatty(1)) {
            mx_output_c(*name_arr);
        }

        // Output single objects directly if output is not a terminal and no specific flags specified
        if (!isatty(1) && flag->C != 1 && flag->X != 1 && flag->m != 1 && flag->l != 1 && flag->g != 1 && flag->o != 1 && flag->is_force != 1) {
            print_single_obj(*name_arr, flag);
        }
    }
}
