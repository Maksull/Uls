#include "../inc/uls.h"

static int a(char *name, t_flag *flag) {
    if (flag->A != 1) {
        return 0;
    }

    return !(mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0);
}

static int count_opened_dirs(t_object **arg, t_flag *flag) {
    int counter = 0;
    t_object *data = *arg;
    DIR *dir_pointer;
    struct dirent *current;

    if (IS_DIR(data->info_st.st_mode) || IS_LNK(data->info_st.st_mode)) {
        if ((dir_pointer = opendir(data->path)) != NULL) {
            while ((current = readdir(dir_pointer)) != NULL) {
                if (current->d_name[0] != '.' || a(current->d_name, flag) == 1) {
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

static t_object *obj_node_init(char *name, char *path) {
    t_object *new_node = (t_object *)malloc(1 * sizeof(t_object));

    new_node->name = mx_strdup(name);
    new_node->path = mx_strdup(path);
    mx_join(&new_node->path, "/");
    mx_join(&new_node->path, name);
    new_node->error = NULL;
    if (lstat(new_node->path, &(new_node->info_st)) == -1) {
        new_node->error = mx_strdup(strerror(errno));
    }

    new_node->next = NULL;
    return new_node;
}

static void process_through_dirs(t_object ***data, t_flag *flag) {
    DIR *dir_pointer;
    struct dirent *current;
    int counter = 0;

    for (int i = 0; (*data)[i] != NULL; i++) {
        counter = count_opened_dirs(&(*data)[i], flag);
        if (counter > 0) {
            (*data)[i]->next = malloc((counter + 1) * sizeof(t_object *));
            if ((dir_pointer = opendir((*data)[i]->path)) != NULL) {
                for (counter = 0; (current = readdir(dir_pointer)) != NULL;) {
                    if (current->d_name[0] != '.' || a(current->d_name, flag) == 1) {
                        (*data)[i]->next[counter++] = 
                        obj_node_init(current->d_name, (*data)[i]->path);
                    }
                }
                (*data)[i]->next[counter] = NULL;
                closedir(dir_pointer);
            }
        }
    }
    mx_output_default(data, flag);
}

void mx_process_dir(t_object ***data, t_flag *flag) {
    t_object **file_arr = mx_get_files(&(*data), flag);

	if (file_arr) {
		mx_output_selector(&file_arr, flag, 0);
		
        if (*data) {
			mx_printchar('\n');
        }

		flag->files = 1;
        mx_free_arr(&file_arr);
	}
    if (*data) {
        process_through_dirs(&(*data), flag);
    }
}
