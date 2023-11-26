#include "../inc/uls.h"

static int compare(t_object *a, t_object *b, t_flag *flag) {
    if (flag->S == 1) {
        if (a->info_st.st_size < b->info_st.st_size) {
            return 1;
        }

        if (a->info_st.st_size == b->info_st.st_size) {
            return mx_strcmp(a->name, b->name);
        }
    } else if (flag->t == 1 && flag->c != 1 && flag->u != 1) {
        if (a->info_st.st_mtime == b->info_st.st_mtime) {
            if (a->info_st.st_mtimespec.tv_nsec == b->info_st.st_mtimespec.tv_nsec) {
                return mx_strcmp(a->name, b->name);
            }

            return (a->info_st.st_mtimespec.tv_nsec < b->info_st.st_mtimespec.tv_nsec) ? 1 : 0;
        }

        return (a->info_st.st_mtime < b->info_st.st_mtime) ? 1 : 0;
    } else if (flag->u == 1 && flag->t == 1 && flag->S != 1) {
        if (a->info_st.st_atime == b->info_st.st_atime) {
            if (a->info_st.st_atimespec.tv_nsec == b->info_st.st_atimespec.tv_nsec) {
                return mx_strcmp(a->name, b->name);
            }

            return (a->info_st.st_atimespec.tv_nsec < b->info_st.st_atimespec.tv_nsec) ? 1 : 0;
        }

        return (a->info_st.st_atime < b->info_st.st_atime) ? 1 : 0;
    } else if (flag->c == 1 && flag->t == 1 && flag->S != 1) {
        if (a->info_st.st_ctime == b->info_st.st_ctime) {
            if (a->info_st.st_ctimespec.tv_nsec == b->info_st.st_ctimespec.tv_nsec) {
                return mx_strcmp(a->name, b->name);
            }

            return (a->info_st.st_ctimespec.tv_nsec < b->info_st.st_ctimespec.tv_nsec) ? 1 : 0;
        }
        
        return (a->info_st.st_ctime < b->info_st.st_ctime) ? 1 : 0;
    } else if (flag->t != 1 && flag->S != 1) {
        if (mx_strcmp(a->name, b->name) > 0) {
            return 1;
        }
    }

    return 0;
}

void mx_sort_objects(t_object ***disp, t_flag *flag) {
	t_object **arr = *disp;
	int size = 0;

    while (arr[size] != NULL) {
        size++;
    }

	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
            if (arr[i]->error != NULL) {
                    if (mx_strcmp(arr[i]->name, arr[j]->name) == 1) {
                        t_object *temp = arr[i];

                        arr[i] = arr[j];
                        arr[j] = temp;
                    }
            } else if (compare(arr[i], arr[j], flag) == flag->reverse) {
                t_object *temp = arr[i];

                arr[i] = arr[j];
                arr[j] = temp;
			}
		}
	}
}
