#include "uls.h"

static bool is_str_flag(char *flag) {
    if (flag[0] == '-' && mx_is_alpha(flag[1])) {
        return true;
    }

    return false;
}

static void validate_flags(char *argv[]) {
    char *flags_str = argv[1] != NULL ? argv[1] : NULL;

    if (flags_str == NULL) {
        return;
    }

    if (!is_str_flag(flags_str)) {
        return;
    }

    for (int i = 1; flags_str[i] != '\0'; i++) {
        if (!mx_str_contains(VALID_FLAGS, flags_str[i])) {
            mx_print_flag_error(flags_str[i]);
            exit(1);
        }
    }
}

static void add_flags(t_flag **flag, char flag_char) {
    if (flag_char == 'T') {
        (*flag)->has_T = 1;
    } else if (flag_char == 'G' && isatty(1)) {
        (*flag)->has_G = 1;
    } else if (flag_char == 'c') {
        (*flag)->has_u = 0;
        (*flag)->has_c = 1;
    } else if (flag_char == 't') {
        (*flag)->has_t = 1;
    } else if (flag_char == 'u') {
        (*flag)->has_u = 1;
        (*flag)->has_c = 0;
    } else if (flag_char == 'S') {
        (*flag)->has_S = 1;
    } else if (flag_char == 'r' && (*flag)->has_reverse != -1) {
        (*flag)->has_reverse = 0;
    } else if (flag_char == 'G' && !isatty(1)) {
        (*flag)->has_G = 0;
    } else if (flag_char == 'R') {
        (*flag)->has_recursion = 1;
    } else if (flag_char == 'A') {
        (*flag)->has_A = 1;
    } else if (flag_char == 'g') {
        (*flag)->has_g = 1;
		(*flag)->has_l = 1;
	} else if (flag_char == 'o') {
        (*flag)->has_o = 1;
		(*flag)->has_l = 1;
	} else {
        (*flag)->has_X = 0;
        (*flag)->has_l = 0;
        (*flag)->has_C = 0;
        (*flag)->has_m = 0;
        (*flag)->is_force = 0;

        if (flag_char == 'l') {
            (*flag)->has_l = 1;
        } else if (flag_char == 'C') {
            (*flag)->has_C = 1;
        } else if (flag_char == '1') {
            (*flag)->is_force = 1;
        } else if (flag_char == 'x') {
            (*flag)->has_X = 1;
        } else if (flag_char == 'm') {
            (*flag)->has_m = 1;
        } else {
            mx_print_flag_error(flag_char);
            free(*flag);
            flag = NULL;
            exit(1);
        }
    } 
} 

t_flag *mx_get_flags(char *argv[], int *i) {
    validate_flags(argv);
    t_flag *flag = malloc(sizeof(t_flag));
    flag->has_reverse = 1;

    while(argv[(*i)]) {
        if (argv[(*i)][0] == '-') {
            if (argv[(*i)][1] == '-') {
                (*i)++;
                break;
            }
            if (argv[(*i)][1] == '\0') {
                break;
            }
            for (int j = 1; argv[(*i)][j]; j++) {
                add_flags(&flag, argv[(*i)][j]);
            }
        } else {
            break;
        }

        (*i)++;
    }

    return flag;
}
