#include "../inc/uls.h"

// Function to print a colored file name based on its type and mode
void mx_print_color_file_name(char* file_name, mode_t mode)
{
    // Extract the file type from the mode
    mode_t modified_mode = mode & S_IFMT;

    // Determine the color based on the file type
    if (modified_mode == S_IFDIR)
    {
        // Directory: Green if sticky bit is set and writable by others, Yellow if writable by others, Blue otherwise
        if (S_ISVTX & mode && mode & S_IWOTH)
        {
            mx_printstr(COLOR_BLACK_GREEN);
        }
        else if (!(S_ISVTX & mode) && mode & S_IWOTH)
        {
            mx_printstr(COLOR_BLACK_YELLOW);
        }
        else
        {
            mx_printstr(COLOR_BLUE_DFLT);
        }
    }
    else if (modified_mode == S_IFLNK)
    {
        // Symbolic Link: Magenta
        mx_printstr(COLOR_MAGENTA_DFLT);
    }
    else if (modified_mode == S_IFSOCK)
    {
        // Socket: Green
        mx_printstr(COLOR_GREEN_DFLT);
    }
    else if (modified_mode == S_IFIFO)
    {
        // FIFO (Named Pipe): Yellow
        mx_printstr(COLOR_YELLOW_DFLT);
    }
    else if (IS_X(mode) && IS_R(mode))
    {
        // Executable: Red if setuid, Cyan if setgid, Red otherwise
        if (S_ISUID & mode)
        {
            mx_printstr(COLOR_BLACK_RED);
        }
        else if (S_ISGID & mode)
        {
            mx_printstr(COLOR_BLACK_CYAN);
        }
        else
        {
            mx_printstr(COLOR_RED_DFLT);
        }
    }
    else if (modified_mode == S_IFBLK)
    {
        // Block Device: Cyan
        mx_printstr(COLOR_BLUE_CYAN);
    }
    else if (modified_mode == S_IFCHR)
    {
        // Character Device: Yellow
        mx_printstr(COLOR_BLUE_YELLOW);
    }

    // Print the file name with the determined color
    mx_printstr(file_name);

    // Reset the color to default
    mx_printstr(COLOR_DEFAULT);
}
