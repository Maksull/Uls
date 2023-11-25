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
    int has_l;
    int has_C;
    int has_X;
    int has_recursion;
    int has_reverse;
    int has_t;
    int has_u;
    int has_c;
    int has_S;
    int has_A;
    int is_force;
    int has_m;
    int has_files;
    int has_G;
    int has_T;
    int has_g;
    int has_o;
    int exit_code;
}               t_flag;

typedef struct s_info {
    int link;
    int size;
    int group;
    int user;
    bool is_develop;
}              t_info;

typedef struct s_entity {
    char *name_str;
    char *path_str;
    char *error;
    struct stat info_st;
    struct s_entity **next;
}              t_entity;

typedef struct s_type {
    int n_f;
    int n_d;
    int n_e;
    int i;
}              t_type;

static const char *VALID_FLAGS = "ACGRSTcfglmortux1";

// flags
t_flag *mx_get_flags(char *argv[], int *i);

// dirs, files etc
void mx_opendir(t_entity ***name_arr, t_flag *flag);
int mx_max_lenngth_name_arr(t_entity **name_arr);
t_entity **mx_get_name_arr(int argc, char **argv, int i);
t_entity **mx_get_files(t_entity ***data, t_flag *flag);
void mx_sort_content(t_entity ***disp, t_flag *flag);
void mx_delete_files(t_entity ***data, t_flag *flag);
void mx_delete_entities(t_entity ***data, t_entity **dirs);
void mx_delete_arr(t_entity ***data);

// Output
void mx_output_selector(t_entity ***name_arr, t_flag *flag, int flag_num);
void mx_output_l(t_entity **name_arr, t_flag *flag, int flag_num);
void mx_output_c(t_entity **name_arr);
void mx_output_x(t_entity **name_arr);
void mx_output_with_color(t_entity **name_arr, t_flag *flag);
void mx_output_m(t_entity **name_arr, t_flag *flag);
void mx_output_default(t_entity ***data, t_flag *flag);

// Print functions
void mx_print_permissions(t_entity *entity);
void mx_print_size(t_entity *entity, t_info *size);
void mx_print_all(t_entity *entity, t_info *size, t_flag *flag);
void mx_printstr_with_color(t_entity *data);
void mx_print_tab(int length, int max_length);

// error handling
void mx_handle_error(t_entity ***error, t_flag *flag);
void mx_print_flag_error(char flag);

// utils
bool mx_is_alpha(char c);
bool mx_str_contains(char *str, char c);
char *mx_get_major_number(t_entity *entity);
char *mx_get_minor_number(t_entity *entity);
t_entity *mx_create_new_file_node(t_entity *arg);

#endif

