#include "../inc/uls.h"

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
        if (!mx_str_contains(FLAGS, flags_str[i])) {
            mx_print_wrong_usage(flags_str[i]);
            exit(1);
        }
    }
}

static void apply_flags(t_flag **flag, char flag_char) {
    if (flag_char == 'T') {
        (*flag)->T = 1;
    } else if (flag_char == 'G' && isatty(1)) {
        (*flag)->G = 1;
    } else if (flag_char == 'c') {
        (*flag)->u = 0;
        (*flag)->c = 1;
    } else if (flag_char == 't') {
        (*flag)->t = 1;
    } else if (flag_char == 'u') {
        (*flag)->u = 1;
        (*flag)->c = 0;
    } else if (flag_char == 'S') {
        (*flag)->S = 1;
    } else if (flag_char == 'r' && (*flag)->reverse != -1) {
        (*flag)->reverse = 0;
    } else if (flag_char == 'G' && !isatty(1)) {
        (*flag)->G = 0;
    } else if (flag_char == 'R') {
        (*flag)->recursion = 1;
    } else if (flag_char == 'A') {
        (*flag)->A = 1;
    } else if (flag_char == 'g') {
        (*flag)->g = 1;
		(*flag)->l = 1;
	} else if (flag_char == 'o') {
        (*flag)->o = 1;
		(*flag)->l = 1;
	} else {
        (*flag)->X = 0;
        (*flag)->l = 0;
        (*flag)->C = 0;
        (*flag)->m = 0;
        (*flag)->is_force = 0;

        if (flag_char == 'l') {
            (*flag)->l = 1;
        } else if (flag_char == 'C') {
            (*flag)->C = 1;
        } else if (flag_char == '1') {
            (*flag)->is_force = 1;
        } else if (flag_char == 'x') {
            (*flag)->X = 1;
        } else if (flag_char == 'm') {
            (*flag)->m = 1;
        } else {
            mx_print_wrong_usage(flag_char);
            free(*flag);
            flag = NULL;
            exit(1);
        }
    } 
} 

t_flag *mx_get_flags_applied(char *argv[], int *i) {
    validate_flags(argv);
    t_flag *flag = malloc(sizeof(t_flag));
    flag->reverse = 1;

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
                apply_flags(&flag, argv[(*i)][j]);
            }
        } else {
            break;
        }

        (*i)++;
    }

    return flag;
}
