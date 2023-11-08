#include "../inc/uls.h"

static void output_acl_params(char** acl_params, int idx) 
{
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

void mx_output_acl_info(const char* acl_str) 
{
    char** acl_lines = mx_strsplit(acl_str, '\n');

    for (int i = 1; acl_lines[i] != NULL; ++i) {

        char** acl_params = mx_strsplit(acl_lines[i], ':');
        output_acl_params(acl_params, i - 1);
        mx_del_strarr(&acl_params);

    }
    mx_del_strarr(&acl_lines);
}
