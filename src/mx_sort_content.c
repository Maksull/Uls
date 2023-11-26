#include "../inc/uls.h"

// Function to compare two objects based on the specified sorting criteria in the flag
static int compare(t_object *a, t_object *b, t_flag *flag) {
    // Sorting by size (-S)
    if (flag->S == 1) {
        if (a->info_st.st_size < b->info_st.st_size) {
            return 1;
        }
        if (a->info_st.st_size == b->info_st.st_size) {
            return mx_strcmp(a->name, b->name);
        }
    } 
    // Sorting by modification time (-t)
    else if (flag->t == 1 && flag->c != 1 && flag->u != 1) {
        if (a->info_st.st_mtime == b->info_st.st_mtime) {
            if (a->info_st.st_mtimespec.tv_nsec == b->info_st.st_mtimespec.tv_nsec) {
                return mx_strcmp(a->name, b->name);
            }
            return (a->info_st.st_mtimespec.tv_nsec < b->info_st.st_mtimespec.tv_nsec) ? 1 : 0;
        }
        return (a->info_st.st_mtime < b->info_st.st_mtime) ? 1 : 0;
    } 
    // Sorting by access time (-t -u)
    else if (flag->u == 1 && flag->t == 1 && flag->S != 1) {
        if (a->info_st.st_atime == b->info_st.st_atime) {
            if (a->info_st.st_atimespec.tv_nsec == b->info_st.st_atimespec.tv_nsec) {
                return mx_strcmp(a->name, b->name);
            }
            return (a->info_st.st_atimespec.tv_nsec < b->info_st.st_atimespec.tv_nsec) ? 1 : 0;
        }
        return (a->info_st.st_atime < b->info_st.st_atime) ? 1 : 0;
    } 
    // Sorting by change time (-t -c)
    else if (flag->c == 1 && flag->t == 1 && flag->S != 1) {
        if (a->info_st.st_ctime == b->info_st.st_ctime) {
            if (a->info_st.st_ctimespec.tv_nsec == b->info_st.st_ctimespec.tv_nsec) {
                return mx_strcmp(a->name, b->name);
            }
            return (a->info_st.st_ctimespec.tv_nsec < b->info_st.st_ctimespec.tv_nsec) ? 1 : 0;
        }
        return (a->info_st.st_ctime < b->info_st.st_ctime) ? 1 : 0;
    } 
    // Sorting by name (default)
    else if (flag->t != 1 && flag->S != 1) {
        if (mx_strcmp(a->name, b->name) > 0) {
            return 1;
        }
    }

    return 0; // Return 0 if no swapping is needed
}

// Function to sort an array of objects based on the specified criteria
void mx_sort_objects(t_object ***disp, t_flag *flag) {
	t_object **arr = *disp;
	int size = 0;

    // Calculate the size of the array
    while (arr[size] != NULL) {
        size++;
    }

    // Bubble sort algorithm to sort the array
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
            // If the current object has an error, sort based on name
            if (arr[i]->error != NULL) {
                if (mx_strcmp(arr[i]->name, arr[j]->name) == 1) {
                    t_object *temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            } 
            // Otherwise, compare based on the specified criteria in the flag
            else if (compare(arr[i], arr[j], flag) == flag->reverse) {
                t_object *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
			}
		}
	}
}
