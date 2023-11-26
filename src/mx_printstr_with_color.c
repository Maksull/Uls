#include "../inc/uls.h"

// Function to print the name with a color based on file mode
static void print_name(t_object* data) {
    mx_printstr(data->name); // Print the file/directory name
    mx_printstr("\x1b[0m"); // Reset color
}

// Function to print with the first set of conditions
static int print_first(t_object* data) {
    mode_t mode = data->info_st.st_mode;

    if (IS_DIR(mode)) { // Directory condition
        mx_printstr("\033[34m"); // Blue color
        print_name(data);
        return 1;
    } else if (mode & S_IXOTH) { // Executable condition
        mx_printstr("\x1b[31m"); // Red color
        print_name(data);
        return 1;
    } else if (IS_LNK(mode)) { // Symbolic link condition
        mx_printstr("\033[35m"); // Purple color
        print_name(data);
        return 1;
    }
    return 0;
}

// Function to print with the second set of conditions
static int print_second(t_object* data) {
    mode_t mode = data->info_st.st_mode;

    if (IS_BLK(mode)) { // Block special file condition
        mx_printstr("\033[34;46m"); // Cyan background
        print_name(data);
        return 1;
    } else if (IS_SOCK(mode)) { // Socket condition
        mx_printstr("\033[32m"); // Green color
        print_name(data);
        return 1;
    } else if (IS_CHR(mode)) { // Character special file condition
        mx_printstr("\033[34;43m"); // Yellow background
        print_name(data);
        return 1;
    }
    return 0;
}

// Main function to print the name with associated colors based on file mode
void mx_printstr_G(t_object* data) {
    mode_t mode = data->info_st.st_mode;

    // Check and print based on the first set of conditions
    if (print_first(data) == 1)
        return;
    
    // Check and print based on the second set of conditions
    if (print_second(data) == 1)
        return;

    // For other file types, print with different colors
    if (IS_FIFO(mode)) // FIFO (named pipe) condition
        mx_printstr("\033[33m"); // Yellow color
    else if (IS_EXEC(mode)) // Executable condition
        mx_printstr("\033[31m"); // Red color
    else if (IS_WHT(mode)) // Whiteout condition
        mx_printstr("\033[36m"); // Cyan color
    else {
        mx_printstr(data->name); // Print the file/directory name without specific color
        return;
    }
    print_name(data);
}
