#include "../inc/uls.h"

// Function to obtain directory entries and store file_info structures in a list
bool mx_get_dir_entries(t_list **entries, const char *name, t_configuration *configuration) {
    DIR *dir = opendir(name);
    if (dir == NULL)
    {
        return false; // Return false if directory cannot be opened
    }

    struct dirent *entry = NULL;

    // Iterate through directory entries and collect file information
    while ((entry = readdir(dir)) != NULL)
    {
        if (!mx_is_ignored(entry->d_name, configuration->ignore_type))
        {
            mx_push_back(entries, mx_get_file_info(name, entry->d_name, configuration));
        }
    }
        
    closedir(dir);
    
    return true; // Return true on successful retrieval of directory entries
}
