#include "../inc/uls.h"

// Function to output special characters based on the file mode
void mx_output_for_F(mode_t mode) 
{
    mode_t modified_mode = mode & S_IFMT; // Extract the file type information

    // Switch statement to determine the file type and output the corresponding character
    switch (modified_mode) {
        case S_IFDIR:
            mx_printchar('/'); // Directory indicator
            break;
        case S_IFLNK:
            mx_printchar('@'); // Symbolic link indicator
            break;
        case S_IFSOCK:
            mx_printchar('='); // Socket indicator
            break;
        case S_IFIFO:
            mx_printchar('|'); // FIFO (named pipe) indicator
            break;
        case S_IFWHT:
            mx_printchar('%'); // Whiteout indicator (for BSD compatibility)
            break;
        default:
            // If executable and readable, output '*'
            if (IS_X(mode) && IS_R(mode)) {
                mx_printchar('*');
            }
            break;
    }
}
