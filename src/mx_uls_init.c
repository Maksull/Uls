#include "../inc/uls.h"

static int handle_reg_files(char** files, t_uls_flags* flags, int* file_count) 
{
    t_file* reg_files = NULL;
    bool dir_exists = false;
    bool has_errors = false;
    for (int i = 0; files[i] != NULL; ++i) {
        t_stat stat;
        if (lstat(files[i], &stat) == 0) {
            if (!IS_DIR(stat.st_mode)) {
                ++(*file_count);
                bool is_prefixed = files[i][0] == '/';
                mx_push_back(&reg_files, is_prefixed ? "" : ".", files[i], flags);
                continue;

            }
            dir_exists = true;
            continue;
        }
        mx_print_dir_error(strerror(errno), files[i]);
        has_errors = true;
    }
    
    if (reg_files != NULL) {
        mx_handle_file_array(&reg_files, NULL, false, true, flags);
        dir_exists ? mx_printstr("\n") : (void) 0;
    }
    mx_clear_list(&reg_files);

    return has_errors ? 1 : 0;
}

static int handle_directories(char** files, int file_count, int dir_count, t_uls_flags* flags) 
{
    t_file* directories = NULL;
    int dir_idx = 0;
    int error_code = 0;

    for (int i = 0; files[i] != NULL; ++i) {
        t_stat stat;
        if (lstat(files[i], &stat) == 0 && IS_DIR(stat.st_mode)) {
            mx_push_back(&directories, ".", files[i], flags);
            if (dir_count > 1 && dir_idx)
                mx_printstr("\n");

            bool is_single = dir_count <= 1 && !file_count;                               
            error_code = mx_handle_dir(files[i], flags, is_single) == 0 ? error_code : 1;
            ++dir_idx;
        }
    }
    mx_clear_list(&directories);

    return error_code;
}

static bool cmp_strings(const char* a, const char* b) 
{
    return mx_strcmp(a, b) > 0;
}

static bool cmp_strings_r(const char* a, const char* b) 
{
    return mx_strcmp(a, b) < 0;
}

static int partition(char **arr, int low, int high, bool (*cmp)(const char* a, const char* b))
{
    char *pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (cmp(arr[j], pivot))
        {
            i++;
            char *temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    char *temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

static void quicksort(char **arr, int low, int high, bool (*cmp)(const char* a, const char* b))
{
    if (low < high)
    {
        int pi = partition(arr, low, high, cmp);

        quicksort(arr, low, pi - 1, cmp);
        quicksort(arr, pi + 1, high, cmp);
    }
}

int mx_uls_init(char** files, int file_count, t_uls_flags* flags) 
{
    quicksort(files, 0, file_count - 1, cmp_strings);
    
    int reg_file_count = 0;
    int error_code = handle_reg_files(files, flags, &reg_file_count);

    flags->r ? quicksort(files, 0, file_count - 1, cmp_strings_r) : (void)0;

    return !handle_directories(files, reg_file_count, file_count - reg_file_count, flags) ? error_code : 1;
}