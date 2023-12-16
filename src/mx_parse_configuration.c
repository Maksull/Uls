#include "../inc/uls.h"

t_configuration *mx_parse_configuration(int argc, char *argv[]) {
    // Allocate memory for the configuration struct
    t_configuration *configuration = malloc(sizeof(t_configuration));
    mx_memset(configuration, 0, sizeof(t_configuration)); // Initialize to default values

    // Check if output is a terminal, set the default format accordingly
    if (isatty(1))
    {
        configuration->format = MULTI_COLUMN;
    }

    // Loop through command-line arguments to parse options
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') // Break loop if it's not an option
        {
            break;
        }
            
        for (char *str = argv[i] + 1; *str != '\0'; str++) {
            // Switch through each option character and set corresponding configuration flags
            switch (*str) {
                // Cases for various flags and options, setting configuration options accordingly
                case 'R':
                    configuration->use_recursion = true;
                    break;
                case 'A':
                    configuration->ignore_type = DOTS;
                    break;
                case 'a':
                    configuration->ignore_type = NOT_IGNORED;
                    break;
                case 'G':
                    configuration->use_colors = true;
                    break;
                case 'F':
                    configuration->classify = true;
                    configuration->add_only_slash_to_directories = false;
                    break;
                case 'p':
                    configuration->add_only_slash_to_directories = true;
                    configuration->classify = false;
                    break;
                case 'r':
                    configuration->sort_reverse = true;
                    break;
                case 'h':
                    configuration->display_human_readable_size = true;
                    break;
                case 'e':
                    configuration->acl = true;
                    break;
                case 'o':
                    configuration->hide_group_information = true;
                    break;
                case 'T':
                    configuration->full_time_info = true;
                    break;
                case '@':
                    configuration->extended_attributes = true;
                    break;
                case 'g':
                    configuration->hide_owner_information = true;
                    break;
                case 'x':
                    configuration->sort_horizontally = true;
                    break;
                case 'n':
                    configuration->display_numeric_IDs = true;
                    break;
                case 'q':
                    configuration->display_non_printable_characters = true;
                    break;
                case 'H':
                    configuration->follow_symbolic_links = true;
                    break;
                case '1':
                    configuration->format = ONE_COLUMN;
                    break;
                case 'C':
                    configuration->format = MULTI_COLUMN;
                    break;
                case 'c':
                    configuration->time_type = LAST_METADATA_CHANGE;
                    break;
                case 'm':
                    configuration->format = STREAM;
                    break;
                case 'l':
                    configuration->format = DETAILED;
                    break;
                case 'u':
                    configuration->time_type = LAST_ACCESS;
                    break;
                case 'U':
                    configuration->time_type = LAST_CREATION;
                    break;
                case 't':
                    configuration->sort_type = BY_TIME;
                    break;
                case 'S':
                    configuration->sort_type = BY_SIZE;
                    break;
                case 'f':
                    configuration->sort_type = UNSORTED;
                    configuration->ignore_type = NOT_IGNORED;
                    break;
                default:
                    // Handling illegal options, printing usage info and exiting if an unknown option is encountered
                    mx_printerr("uls: illegal option -- ");
                    write(2, str, 1);
                    write(2, "\n", 1);
                    mx_printerr("usage: uls [-@ACFGHRSTUacefghlmnopqrtux1] [file ...]\n");
                    exit(1);
            }
        }
    }

    // Check if output is a terminal to adjust additional display settings
    if (isatty(1))
    {
        configuration->display_non_printable_characters = true;
    }
    else
    {
        configuration->use_colors = false;
    }

    // Adjust follow symbolic links if certain conditions are met
    if (!configuration->follow_symbolic_links)
    {
        configuration->follow_symbolic_links = configuration->format != DETAILED && !configuration->use_colors && !configuration->classify;
    }

    // Adjust ignore type if sorting is unsorted
    if (configuration->sort_type == UNSORTED)
    {
        configuration->ignore_type = NOT_IGNORED;
    }

    // Return the populated configuration struct
    return configuration;
}
