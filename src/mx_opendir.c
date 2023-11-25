#include "uls.h"

static int has_a(char *name, t_flag *flag) {
    if (flag->has_A != 1) {
        return 0;
    }

    return !(mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0);
}

static int count_opened(t_entity **arg, t_flag *flag) {
    int counter = 0;
    t_entity *data = *arg;
    DIR *dir_pointer;
    struct dirent *current;

    if (IS_DIR(data->info_st.st_mode) || IS_LNK(data->info_st.st_mode)) {
        if ((dir_pointer = opendir(data->path_str)) != NULL) {
            while ((current = readdir(dir_pointer)) != NULL) {
                if (current->d_name[0] != '.' || has_a(current->d_name, flag) == 1) {
                    counter++;
                }
            }
            closedir(dir_pointer);
        }
        else {
            (*arg)->error = mx_strdup(strerror(errno));
            flag->exit_code = 1;

            return -1;
        }
    }

    return counter;
}

static t_entity *create_new_node(char *name, char *path) {
    t_entity *new_node = (t_entity *)malloc(1 * sizeof(t_entity));

    new_node->name_str = mx_strdup(name);
    new_node->path_str = mx_strdup(path);
    mx_join(&new_node->path_str, "/");
    mx_join(&new_node->path_str, name);
    new_node->error = NULL;
    if (lstat(new_node->path_str, &(new_node->info_st)) == -1) {
        new_node->error = mx_strdup(strerror(errno));
    }

    new_node->next = NULL;
    return new_node;
}

static void open_dir(t_entity ***data, t_flag *flag) {
    DIR *dir_pointer;
    struct dirent *current;
    int counter = 0;

    for (int i = 0; (*data)[i] != NULL; i++) {
        counter = count_opened(&(*data)[i], flag);
        if (counter > 0) {
            (*data)[i]->next = malloc((counter + 1) * sizeof(t_entity *));
            if ((dir_pointer = opendir((*data)[i]->path_str)) != NULL) {
                for (counter = 0; (current = readdir(dir_pointer)) != NULL;) {
                    if (current->d_name[0] != '.' || has_a(current->d_name, flag) == 1) {
                        (*data)[i]->next[counter++] = 
                        create_new_node(current->d_name, (*data)[i]->path_str);
                    }
                }
                (*data)[i]->next[counter] = NULL;
                closedir(dir_pointer);
            }
        }
    }
    mx_output_default(data, flag);
}

void mx_opendir(t_entity ***data, t_flag *flag) {
    t_entity **file_arr = mx_get_files(&(*data), flag);

	if (file_arr) {
		mx_output_selector(&file_arr, flag, 0);
		
        if (*data) {
			mx_printchar('\n');
        }

		flag->has_files = 1;
        mx_delete_arr(&file_arr);
	}
    if (*data) {
        open_dir(&(*data), flag);
    }
}
