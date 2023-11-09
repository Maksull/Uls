#include "../inc/uls.h"

static void flag_init(t_uls_flags* flags) 
{
    for (int i = 0; i < FLAG_COUNT; ++i) {

        ((&flags->a)[i]) = 0;
    }
}

// Check if a character is an alphabet letter.
static bool isalpha(int c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    {
        return true;
    }
    
    return false;
}

static bool is_flag_found(char flag) 
{
    for (size_t i = 0; i < FLAG_COUNT; ++i) {
        if (flag == const_uls_flags[i])
        {
            return true;
        }
    }

    return false;
}

static t_uls_flags* get_uls_flags(int argc, const char** argv, int* flag_count) 
{
    t_uls_flags* flags = malloc(sizeof(t_uls_flags));
    flag_init(flags);
    for (int i = 1; i < argc; ++i) {
        if ((argv[i][0] == '-') && (mx_strlen(argv[i]) > 1)) {
            if (isalpha(argv[i][1]) || argv[i][1] == '1' || argv[i][1] == '@') {
                ++(*flag_count);
                for (int j = 1; argv[i][j] != '\0'; j++) {
                    if (is_flag_found(argv[i][j])) {
                        mx_add_flag(&flags, argv[i][j]);
                    } else {
                        mx_print_flag_err(argv[i][j]);
                        exit(1);
                    }
                }

            } else {
                mx_print_flag_err(argv[i][1]);
                exit(1);
            }

        } else {
            break;
        }
    }

    return flags;
}

static char** get_file_array(int argc, const char** argv, int flag_count) 
{
    int file_count = argc - flag_count + 1; 
    char** files = malloc(sizeof(char*) * file_count);

    int idx = 0;
    for (int i = flag_count; i < argc; ++i) {

        files[idx++] = mx_strdup(argv[i]);

    }
    files[idx] = NULL;

    return files;
}

static char** get_root_file_array() 
{
    char** files = malloc(sizeof(char*) * 2);

    int idx = 0;
    files[idx++] = mx_strdup(".");
    files[idx] = NULL;

    return files;
}

int main(int argc, char const *argv[]) 
{
    char** files = NULL;
    int flag_count = 1;
    t_uls_flags* flags = get_uls_flags(argc, argv, &flag_count);
    int file_count = argc - flag_count;
        
    if (file_count == 0) {
        files = get_root_file_array();
    } else {
        files = get_file_array(argc, argv, flag_count);
    }
    
    int error_code = mx_uls_init(files, file_count, flags);
    
    free(flags);
    mx_del_strarr(&files);
    
    return error_code;
}
