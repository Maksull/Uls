#include "../inc/uls.h"

// Checks if the given string is a valid flag
static bool is_str_flag(char *flag) {
    if (flag[0] == '-' && mx_is_alpha(flag[1])) {
        return true;
    }
    return false;
}

// Validates the flags provided as command-line arguments
static void validate_flags(char *argv[]) {
    char *flags_str = argv[1] != NULL ? argv[1] : NULL;

    if (flags_str == NULL) {
        return; // No flags provided
    }

    if (!is_str_flag(flags_str)) {
        return; // Invalid flag format
    }

    // Check if each character in the flag string is a valid flag
    for (int i = 1; flags_str[i] != '\0'; i++) {
        if (!mx_str_contains(FLAGS, flags_str[i])) {
            mx_print_wrong_usage(flags_str[i]); // Print error for invalid flag
            exit(1);
        }
    }
}

// Applies the corresponding settings based on the given flag character
static void apply_flags(t_flag **flag, char flag_char) {
    // Handle special cases based on the flag character
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
        // Handle generic cases for flags
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
            mx_print_wrong_usage(flag_char); // Print error for unrecognized flag
            free(*flag);
            flag = NULL;
            exit(1);
        }
    }
}

// Retrieves and applies flags from the command-line arguments
t_flag *mx_get_flags_applied(char *argv[], int *i) {
    validate_flags(argv); // Validate flags before processing
    t_flag *flag = malloc(sizeof(t_flag));
    flag->reverse = 1;

    while(argv[(*i)]) {
        if (argv[(*i)][0] == '-') {
            if (argv[(*i)][1] == '-') {
                (*i)++;
                break; // End of flags, move to file arguments
            }
            if (argv[(*i)][1] == '\0') {
                break; // End of flags, move to file arguments
            }
            for (int j = 1; argv[(*i)][j]; j++) {
                apply_flags(&flag, argv[(*i)][j]); // Apply settings based on flag characters
            }
        } else {
            break; // End of flags, move to file arguments
        }

        (*i)++;
    }

    return flag; // Return the flag settings
}
