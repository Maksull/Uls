#include "../inc/uls.h"

static char **name_arr_init(int argc, char **argv, int i, int *count) {
    int a = i;
    char **name_arr = NULL;

    if (i == argc) {
        *count = 2;
        name_arr = malloc(2 * sizeof(char *));
        name_arr[0] = mx_strdup(".");
        name_arr[1] = NULL;
    }
    else {
        for (; argv[a]; a++) {
        }
        name_arr = malloc((a - i + 1) * sizeof(char *));
        for(a = 0; argv[i]; i++, a++) {
            name_arr[a] = mx_strdup(argv[i]);
        }
        name_arr[a] = NULL;
        *count = a + 1;
    }
    return name_arr;
}

static t_object *obj_node_in_list_init(char *data) {
    t_object *new_node = (t_object *)malloc(1 * sizeof(t_object));

    new_node->error = NULL;
    new_node->path = mx_strdup(data);
    new_node->name = mx_strdup(data);

    if (lstat(data, &(new_node->info_st)) == -1) {
        new_node->error = mx_strdup(strerror(errno));	
    }

    new_node->next = NULL;

    return new_node;
}

static t_object **list_init(char **name, int count) {
    t_object **new_list = malloc(count * sizeof(t_object *));
    int a = 0;

    for (a = 0; name[a]; a++) {
        new_list[a] = obj_node_in_list_init(name[a]);
    }

    new_list[a] = NULL;
    return new_list;
}

t_object **mx_get_name_arr(int argc, char **argv, int i) {
    int counter = 0;
    char **name = name_arr_init(argc, argv, i, &counter);
    t_object **data = list_init(name, counter);

    mx_del_strarr(&name);

    return data;
}
