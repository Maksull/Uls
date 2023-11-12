#include "../inc/uls.h"

// Function to output ACL parameters based on the parsed ACL string
static void output_acl_params(char** acl_params, int idx) 
{
    // Print a space, index, colon, ACL type, colon, and permissions information
    mx_printchar(' ');
    mx_printint(idx);
    mx_printstr(": ");
    mx_printstr(acl_params[0]);
    mx_printchar(':');
    mx_printstr(acl_params[2]);
    mx_printchar(' ');
    mx_printstr(acl_params[4]);
    mx_printchar(' ');
    mx_printstr(acl_params[5]);
    mx_printchar('\n');
}

// Function to extract and output ACL information from the ACL string
void mx_output_acl_info(const char* acl_str) 
{
    // Split the ACL string into individual lines
    char** acl_lines = mx_strsplit(acl_str, '\n');

    // Loop through each line of the ACL information
    for (int i = 1; acl_lines[i] != NULL; ++i) {
        // Split the line into individual ACL parameters
        char** acl_params = mx_strsplit(acl_lines[i], ':');

        // Output ACL parameters
        output_acl_params(acl_params, i - 1);

        // Free the allocated memory for ACL parameters
        mx_del_strarr(&acl_params);
    }

    // Free the allocated memory for ACL lines
    mx_del_strarr(&acl_lines);
}
