#include "../inc/uls.h"

// Function to check if a file is the executable uls file itself
static bool is_uls_file(const char* file, const char* dir) 
{
    bool to_return = false;

    // Get the stat information for the uls executable
    t_stat uls_stat;
    lstat(EXE_STR, &uls_stat);

    // Get the full path of the current file
    char* path = mx_get_file_path(dir, file);

    // Get the stat information for the current file
    t_stat stat;
    lstat(path, &stat);

    // Check if the inode of the current file matches the inode of the uls executable
    if (stat.st_ino == uls_stat.st_ino)
        to_return = true;

    // Free the allocated memory for the file path
    mx_strdel(&path);

    return to_return;
}

// Function to handle a directory, listing its contents based on specified flags
int mx_handle_dir(const char* dir_name, t_uls_flags* flags, bool is_single) 
{
    // Initialize error_code to track directory handling success or failure
    static int error_code = 0;

    // Open the directory for reading
    DIR* dir = opendir(dir_name);
    if (!dir) {
        // Handle directory open errors
        if (errno == EACCES) {
            mx_print_perm_dir_error(strerror(errno), dir_name, is_single);
            return (error_code = 1);
        }

        mx_print_dir_error(strerror(errno), dir_name);
        return (error_code = 1);
    }

    // Directory entry pointer
    t_dirent* dp;

    // List to store file information within the directory
    t_file* inner_files = NULL;

    // Loop through directory entries
    while ((dp = readdir(dir)) != NULL) {
        // Skip files not matching applied uls flags or the uls executable itself
        if (!mx_is_uls_flags_applied(flags, dp->d_name) || is_uls_file(dp->d_name, dir_name))
        {
            continue;  
        }

        // Add file information to the list
        mx_push_back(&inner_files, dir_name, dp->d_name, flags);
    }

    // Close the directory after reading
    closedir(dir);

    // Process the file information list
    mx_handle_file_array(&inner_files, dir_name, true, is_single, flags);

    // Handle recursive flag (-R) to list subdirectories
    t_file* curr_dir_file = inner_files;
    while (curr_dir_file && flags->R) {
        if (IS_DIR(curr_dir_file->stat.st_mode) &&
            !mx_is_implied_dir(curr_dir_file->name)) {
            
            // If it's a directory, recursively handle it
            mx_printstr("\n");
            mx_handle_dir(curr_dir_file->path, flags, false);
        }
        curr_dir_file = curr_dir_file->next;
    }

    // Clear the list of file information
    mx_clear_list(&inner_files);
    
    return error_code;
}
