#include "../inc/uls.h"

static bool is_uls_file(const char* file, const char* dir) 
{
    bool to_return = false;
    t_stat uls_stat;
    lstat(EXE_STR, &uls_stat);

    char* path = mx_get_file_path(dir, file);
    t_stat stat;
    lstat(path, &stat);

    if (stat.st_ino == uls_stat.st_ino)
        to_return = true;

    mx_strdel(&path);

    return to_return;

}

int mx_handle_dir(const char* dir_name, t_uls_flags* flags, bool is_single) 
{
    static int error_code = 0;
    DIR* dir;
    if ((dir = opendir(dir_name)) == NULL) {
        if (errno == EACCES) {
            mx_print_perm_dir_error(strerror(errno), dir_name, is_single);
            return (error_code = 1);
        }

        mx_print_dir_error(strerror(errno), dir_name);
        return (error_code = 1);
    }
    t_dirent* dp;
    t_file* inner_files = NULL;

    while ((dp = readdir(dir)) != NULL) {

        if (!mx_is_uls_flags_applied(flags, dp->d_name) || is_uls_file(dp->d_name, dir_name))
        {
            continue;  
        }

        mx_push_back(&inner_files, dir_name, dp->d_name, flags);

    }
    closedir(dir);
    mx_handle_file_array(&inner_files, dir_name, true, is_single, flags);

    t_file* curr_dir_file = inner_files;
    while (curr_dir_file && flags->R) {

        if (IS_DIR(curr_dir_file->stat.st_mode) &&
            !mx_is_implied_dir(curr_dir_file->name)) {
            
            mx_printstr("\n");
            mx_handle_dir(curr_dir_file->path, flags, false);
        }
        curr_dir_file = curr_dir_file->next;

    }
    mx_clear_list(&inner_files);
    
    return error_code;
}
