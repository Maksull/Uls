#include "../inc/uls.h"

// Function to initialize flags to default values
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

// Check if the provided flag is one of the valid flags
static bool is_flag_found(char flag) 
{
    for (size_t i = 0; i < FLAG_COUNT; ++i) {
        if (flag == const_uls_flags[i]) {
            return true;
        }
    }

    return false;
}

// Parse command line arguments to extract and initialize flags
static t_uls_flags* get_uls_flags(int argc, const char** argv, int* flag_count) 
{
    // Allocate memory for flags and initialize them
    t_uls_flags* flags = malloc(sizeof(t_uls_flags));
    flag_init(flags);

    // Loop through command line arguments to find flags
    for (int i = 1; i < argc; ++i) {
        if ((argv[i][0] == '-') && (mx_strlen(argv[i]) > 1)) {
            if (isalpha(argv[i][1]) || argv[i][1] == '1' || argv[i][1] == '@') {
                ++(*flag_count);

                // Loop through individual characters in the flag
                for (int j = 1; argv[i][j] != '\0'; j++) {
                    // Check if the character is a valid flag
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

// Extract file names from command line arguments
static char** get_file_array(int argc, const char** argv, int flag_count) 
{
    int file_count = argc - flag_count + 1; 
    char** files = malloc(sizeof(char*) * file_count);

    int idx = 0;
    // Loop through command line arguments to extract file names
    for (int i = flag_count; i < argc; ++i) {
        files[idx++] = mx_strdup(argv[i]);
    }
    files[idx] = NULL;

    return files;
}

// Create an array containing the current directory as the only file
static char** get_root_file_array() 
{
    char** files = malloc(sizeof(char*) * 2);

    int idx = 0;
    files[idx++] = mx_strdup(".");
    files[idx] = NULL;

    return files;
}

// Entry point of the program
int main(int argc, char const *argv[]) 
{
    char** files = NULL;
    int flag_count = 1;
    // Parse and initialize flags
    t_uls_flags* flags = get_uls_flags(argc, argv, &flag_count);
    int file_count = argc - flag_count;
        
    // If no file is specified, use the current directory
    if (file_count == 0) {
        files = get_root_file_array();
    } else {
        // Extract file names from command line arguments
        files = get_file_array(argc, argv, flag_count);
    }
    
    // Initialize and run the uls program
    int error_code = mx_uls_init(files, file_count, flags);
    
    // Free allocated memory
    free(flags);
    mx_del_strarr(&files);
    
    return error_code;
}
