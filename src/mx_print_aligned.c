#include "../inc/uls.h"

// Function to print a string aligned within a specified width
void mx_print_aligned(char *str, int width, bool align_right) {
    // Print the string aligned within the specified width
    // If align_right is true, align the string to the right; otherwise, align to the left
    if (!align_right)
    {
       mx_printstr(str); 
    }
        
    for (int i = 0; i < width - mx_strlen(str); i++)
    {
        mx_printchar(' ');
    }
    if (align_right)
    {
        mx_printstr(str);
    }
}
