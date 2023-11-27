#ifndef ULS_H
#define ULS_H

#include "../libmx/inc/libmx.h"
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/acl.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define IS_BLK(mode) (((mode) & S_IFMT) == S_IFBLK)
#define IS_CHR(mode) (((mode) & S_IFMT) == S_IFCHR)
#define IS_DIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#define IS_LNK(mode) (((mode) & S_IFMT) == S_IFLNK)
#define IS_SOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)
#define IS_FIFO(mode) (((mode) & S_IFMT) == S_IFIFO)
#define IS_WHT(mode) (((mode) & S_IFMT) == S_IFWHT)
#define IS_REG(mode) (((mode) & S_IFMT) == S_IFREG)
#define IS_EXEC(mode) ((mode) & S_IXUSR)

typedef struct s_flag {
    int A;
    int C;
    int G;
    int S;
    int T;
    int c;
    int g;
    int l;
    int m;
    int o;
    int t;
    int u;
    int X;
    int recursion;
    int reverse;
    int files;
    int is_force;
    int exit_code;
} t_flag;

typedef struct s_obj_info {
    int link;
    int size;
    int group;
    int user;
    bool is_develop;
} t_obj_info;

typedef struct s_object {
    char *name;
    char *path;
    char *error;
    struct stat info_st;
    struct s_object **next;
} t_object;

typedef struct s_check_struct {
    int check_files;
    int check_dirs;
    int check_errors;
    int i;
} t_check_struct;

static char *FLAGS = "ACGRSTcfglmortux1";

// Flags
t_flag *mx_get_flags_applied(char *argv[], int *i);

// Processng dirs and files
void mx_process_dir(t_object ***name_arr, t_flag *flag);
int mx_get_max_length_name(t_object **name_arr);
t_object **mx_get_name_arr(int argc, char **argv, int i);
t_object **mx_get_files(t_object ***data, t_flag *flag);
void mx_sort_objects(t_object ***disp, t_flag *flag);
void mx_free_files(t_object ***data, t_flag *flag);
void mx_free_entities(t_object ***data, t_object **dirs);
void mx_free_arr(t_object ***data);

// Output functions
void mx_output_selector(t_object ***name_arr, t_flag *flag, int flag_num);
void mx_output_l(t_object **name_arr, t_flag *flag, int flag_num);
void mx_output_c(t_object **name_arr);
void mx_output_x(t_object **name_arr);
void mx_output_G(t_object **name_arr, t_flag *flag);
void mx_output_m(t_object **name_arr, t_flag *flag);
void mx_output_default(t_object ***data, t_flag *flag);

// Print functions
void mx_print_permissions(t_object *obj);
void mx_print_obj_info(t_object *obj, t_obj_info *size);
void mx_print_all(t_object *obj, t_obj_info *size, t_flag *flag);
void mx_printstr_G(t_object *data);
void mx_print_tab(int length, int max_length);

// Error handling
void mx_printerr(const char *s);
void mx_print_invalid_dir(t_object ***error, t_flag *flag);
void mx_print_wrong_usage(char flag);

// Util functions
void mx_join(char **res, char *s2);
bool mx_is_alpha(char c);
bool mx_str_contains(char *str, char c);
char *mx_get_device_ID_major_number(t_object *obj);
char *mx_get_device_ID_minor_number(t_object *obj);
t_object *mx_create_new_file_node(t_object *arg);

#endif

