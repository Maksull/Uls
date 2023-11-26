#include "../inc/uls.h"

// Initializes the name array based on command-line arguments
static char **name_arr_init(int argc, char **argv, int i, int *count) {
    int a = i;
    char **name_arr = NULL;

    // If no arguments after options, default to the current directory
    if (i == argc) {
        *count = 2;
        name_arr = malloc(2 * sizeof(char *));
        name_arr[0] = mx_strdup(".");
        name_arr[1] = NULL;
    } else {
        // Count the number of arguments after options
        for (; argv[a]; a++) {
        }
        // Allocate memory for the name array and copy arguments
        name_arr = malloc((a - i + 1) * sizeof(char *));
        for (a = 0; argv[i]; i++, a++) {
            name_arr[a] = mx_strdup(argv[i]);
        }
        name_arr[a] = NULL;
        *count = a + 1;
    }
    return name_arr;
}

// Initializes a new node for the object list
static t_object *obj_node_in_list_init(char *data) {
    t_object *new_node = (t_object *)malloc(1 * sizeof(t_object));

    new_node->error = NULL;
    new_node->path = mx_strdup(data);
    new_node->name = mx_strdup(data);

    // Fetches information about the file/directory
    if (lstat(data, &(new_node->info_st)) == -1) {
        new_node->error = mx_strdup(strerror(errno));	
    }

    new_node->next = NULL;

    return new_node;
}

// Initializes the object list using the name array
static t_object **list_init(char **name, int count) {
    t_object **new_list = malloc(count * sizeof(t_object *));
    int a = 0;

    // Creates nodes for each entry in the name array
    for (a = 0; name[a]; a++) {
        new_list[a] = obj_node_in_list_init(name[a]);
    }

    new_list[a] = NULL;
    return new_list;
}

// Retrieves and prepares the list of objects from command-line arguments
t_object **mx_get_name_arr(int argc, char **argv, int i) {
    int counter = 0;
    char **name = name_arr_init(argc, argv, i, &counter); // Initialize the name array
    t_object **data = list_init(name, counter); // Initialize the object list

    mx_del_strarr(&name); // Clean up the name array

    return data; // Return the list of objects
}
