#include "../inc/uls.h"

// Function to output a file name for the "-m" flag format
static void output_name_for_m(t_file* file, t_uls_flags* flags, bool is_last_file)
{
    // Output file name with optional color (if enabled)
    flags->G && isatty(1) ? mx_print_color_file_name(file->name, file->stat.st_mode) : mx_printstr(file->name);
    flags->F ? mx_output_for_F(file->stat.st_mode) : (void) 0; // Output special file indicator

    // Output a comma and space if the file is not the last one; otherwise, move to the next line
    !is_last_file ? mx_printstr(", ") : mx_printstr("\n");
}

// Function to output file names in a comma-separated list for the "-m" flag
void mx_output_for_m(t_file** files, t_uls_flags* flags)
{
    int out_width;

    // Determine the output width based on terminal size or use a default width
    if (isatty(1))
    {
        struct winsize win_size;
        ioctl(1, TIOCGWINSZ, &win_size);
        out_width = win_size.ws_col;
    } else
    {
        out_width = CAT_WIDTH;
    }

    t_file* file = *files;
    int counter = 0;

    // Iterate through the files and output names in a comma-separated format
    while (file) {
        // Update the counter with the length of the current file name and separators
        counter += file->next == NULL ? mx_strlen(file->name) : mx_strlen(file->name) + 2;

        // Move to the next line if the counter exceeds the output width
        if (counter >= out_width)
        {
            counter = mx_strlen(file->name) + 2;
            mx_printstr("\n");
        }

        // Output the file name with the appropriate format
        output_name_for_m(file, flags, file->next == NULL);
        file = file->next;
    }
}
