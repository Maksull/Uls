#include "../inc/uls.h"

// Function to output extended attributes of a file
void mx_output_file_xattr(const char* path, bool is_h_on) 
{
    char attr_name[XATTR_LEN];
    char value[XATTR_LEN];
    // Get the list of extended attributes for the given file path
    int list_len = listxattr(path, attr_name, XATTR_LEN, XATTR_NOFOLLOW);
    int value_len = 0;

    // Iterate through the list of extended attributes
    for (int idx = 0; idx < list_len;) {
        mx_printstr("\t"); // Add indentation
        mx_printstr(&attr_name[idx]); // Print the attribute name

        // Get the value of the extended attribute
        value_len = getxattr(path, &attr_name[idx], value, XATTR_LEN, 0, 0);

        mx_printstr("\t   "); // Add spacing
        if (value_len != -1)
        {
            mx_printint(value_len); // Print the length of the attribute value
        }
        else {
            mx_printint(0); // If value length is -1, print 0
        }

        is_h_on ? mx_printchar('B') : (void) 0; // Print 'B' if -h flag is set
        mx_printstr(" \n"); // Move to the next line
        idx += mx_strlen(&attr_name[idx]) + 1; // Move to the next attribute in the list
    }
}
