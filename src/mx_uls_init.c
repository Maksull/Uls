#include "../inc/uls.h"

// Handle regular files and build a list for them
static int handle_reg_files(char** files, t_uls_flags* flags, int* file_count) {
    // Initialize an empty list for regular files
    t_file* reg_files = NULL;
    bool dir_exists = false;
    bool has_errors = false;

    // Iterate through the array of files
    for (int i = 0; files[i] != NULL; ++i) {
        t_stat stat;
        // Get file status using lstat
        if (lstat(files[i], &stat) == 0) {
            // Check if it is not a directory
            if (!IS_DIR(stat.st_mode)) {
                ++(*file_count);
                // Add the file to the list
                bool is_prefixed = files[i][0] == '/';
                mx_push_back(&reg_files, is_prefixed ? "" : ".", files[i], flags);
                continue;
            }
            dir_exists = true;
            continue;
        }
        // Print directory error if lstat fails
        mx_print_dir_error(strerror(errno), files[i]);
        has_errors = true;
    }

    // Handle regular files list
    if (reg_files != NULL) {
        mx_handle_file_array(&reg_files, NULL, false, true, flags);
        dir_exists ? mx_printstr("\n") : (void) 0;
    }
    mx_clear_list(&reg_files);

    // Return 1 if there were errors, 0 otherwise
    return has_errors ? 1 : 0;
}

// Handle directories
static int handle_directories(char** files, int file_count, int dir_count, t_uls_flags* flags) {
    // Initialize a list for directories
    t_file* directories = NULL;
    int dir_idx = 0;
    int error_code = 0;

    // Iterate through the array of files
    for (int i = 0; files[i] != NULL; ++i) {
        t_stat stat;
        // Get file status using lstat
        if (lstat(files[i], &stat) == 0 && IS_DIR(stat.st_mode)) {
            // Add the directory to the list
            mx_push_back(&directories, ".", files[i], flags);
            if (dir_count > 1 && dir_idx)
                mx_printstr("\n");

            bool is_single = dir_count <= 1 && !file_count;
            // Handle the directory
            error_code = mx_handle_dir(files[i], flags, is_single) == 0 ? error_code : 1;
            ++dir_idx;
        }
    }
    mx_clear_list(&directories);

    // Return the error code
    return error_code;
}

// Compare strings for quicksort
static bool cmp_strings(const char* a, const char* b) {
    return mx_strcmp(a, b) > 0;
}

// Compare strings in reverse for quicksort
static bool cmp_strings_r(const char* a, const char* b) {
    return mx_strcmp(a, b) < 0;
}

// Partition function for quicksort
static int partition(char** arr, int low, int high, bool (*cmp)(const char* a, const char* b)) {
    char* pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (cmp(arr[j], pivot)) {
            i++;
            char* temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    char* temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Quicksort function for strings
static void quicksort(char** arr, int low, int high, bool (*cmp)(const char* a, const char* b)) {
    if (low < high) {
        int pi = partition(arr, low, high, cmp);

        quicksort(arr, low, pi - 1, cmp);
        quicksort(arr, pi + 1, high, cmp);
    }
}

// Initialize the uls program with the given files and flags
int mx_uls_init(char** files, int file_count, t_uls_flags* flags) {
    // Sort files alphabetically
    quicksort(files, 0, file_count - 1, cmp_strings);

    int reg_file_count = 0;
    // Handle regular files and count them
    int error_code = handle_reg_files(files, flags, &reg_file_count);

    // Sort files in reverse if -r flag is present
    flags->r ? quicksort(files, 0, file_count - 1, cmp_strings_r) : (void)0;

    // Handle directories and return the final error code
    return !handle_directories(files, reg_file_count, file_count - reg_file_count, flags) ? error_code : 1;
}
