#include "uls.h"

static char **build_name_arr_arr(int argc, char **argv, int i, int *count) {
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

static t_entity *create_new_li_node(char *data) {
    t_entity *new_node = (t_entity *)malloc(1 * sizeof(t_entity));

    new_node->error = NULL;
    new_node->path_str = mx_strdup(data);
    new_node->name_str = mx_strdup(data);

    if (lstat(data, &(new_node->info_st)) == -1) {
        new_node->error = mx_strdup(strerror(errno));	
    }

    new_node->next = NULL;

    return new_node;
}

static t_entity **build_list(char **name, int count) {
    t_entity **new_list = malloc(count * sizeof(t_entity *));
    int a = 0;

    for (a = 0; name[a]; a++) {
        new_list[a] = create_new_li_node(name[a]);
    }

    new_list[a] = NULL;
    return new_list;
}

t_entity **mx_get_name_arr(int argc, char **argv, int i) {
    int counter = 0;
    char **name = build_name_arr_arr(argc, argv, i, &counter);
    t_entity **data = build_list(name, counter);

    mx_del_strarr(&name);

    return data;
}
