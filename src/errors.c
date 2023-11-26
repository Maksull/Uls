#include "../inc/uls.h"

// Prints an error message for wrong usage of flags
void mx_print_wrong_usage(char flag) {
    // Print an error message indicating illegal option
    mx_printerr("uls: illegal option -- ");
    write(2, &flag, 1);
    mx_printerr("\n");

    // Print the correct usage for the uls command
    mx_printerr("usage: uls [-");
    mx_printerr(FLAGS);
    mx_printerr("] [file ...]\n");
}

// Prints error messages for invalid directories and frees memory
void mx_print_invalid_dir(t_object ***err, t_flag *flag) {
    // Check if the error object exists and contains errors
    if (err && *err && **err) {
        // Sort the error objects and set flag values
        mx_sort_objects(err, flag);
        flag->files = 1;
        flag->exit_code = 1;

        // Loop through each error object and print error messages
        for (int i = 0; (*err)[i]; i++) {
            // Create an error message for the current object
            char *error = mx_strjoin(mx_strjoin("uls: ", (*err)[i]->name), ": ");
            error = mx_strjoin(mx_strjoin(error, (*err)[i]->error), "\n");

            // Print the error message
            mx_printerr(error);

            // Free memory allocated for the current error object's fields
            mx_strdel(&(*err)[i]->name);
            mx_strdel(&(*err)[i]->path);
            mx_strdel(&(*err)[i]->error);
            free((*err)[i]);
            (*err)[i] = NULL;
        }

        // Free memory allocated for the error object array and set it to NULL
        free(*err);
        *err = NULL;
    }
}
