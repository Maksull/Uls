#ifndef ULS_H
#define ULS_H

// Standard C Library Headers
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// Custom Library Headers
#include "../libmx/inc/libmx.h"
#include "const.h"

// Definition of structures for handling file information and command-line flags
typedef struct dirent t_dirent;
typedef struct stat t_stat;
typedef struct passwd t_passwd;
typedef struct group t_group;

typedef struct s_uls_flags {
    int a;   // Show hidden files
    int A;   // Show hidden files, excluding '.' and '..'
    int G;   // Enable colorized output
    int l;   // List format
    int r;   // Reverse order
    int t;   // Sort by modification time
    int c;   // Use time of last status change for sorting
    int u;   // Use time of last access for sorting
    int S;   // Sort by file size
    int T;   // Display complete time information
    int h;   // Human-readable sizes
    int at;  // Display file type symbol after name
    int e;   // Print Access Control List (ACL)
    int R;   // Recursively list subdirectories encountered
    int C;   // Force multi-column output
    int one; // Force output to be one entry per line
    int F;   // Append indicator (one of */=@|) to entries
    int f;   // Disable sorting; display entries in directory order
    int m;   // Stream output format; list files across the page
} t_uls_flags;

typedef struct s_file {
    char* num_links;      // Number of hard links
    char* user_name;      // Name of the file owner
    char* group_name;     // Name of the file group
    char* size;           // Size of the file
    char* lm_day;         // Last modification day
    char* lm_time;        // Last modification time
    char* lm_month;       // Last modification month
    char* perms;          // File permissions
    char* linked_file;    // Linked file (for symbolic links)
    char* acl_str;        // Access Control List (ACL) string

    char* name;           // File name
    char* path;           // Full path of the file
    char type;            // File type
    t_stat stat;          // File statistics

    struct s_file* next;  // Pointer to the next file in the linked list
} t_file;

// Definitions of macro functions for checking file attributes
#define IS_R(m) (m & S_IRUSR && m & S_IRGRP && m & S_IROTH)
#define IS_X(m) (m & S_IXUSR && m & S_IXGRP && m & S_IXOTH)
#define IS_DIR(m) (((m) & 00170000) == 0040000)
#define IS_LNK(m) (((m) & S_IFMT) == S_IFLNK)
#define IS_REG(m) (((m) & S_IFMT) == S_IFREG)

// Function prototypes for file utilities
int mx_uls_init(char** files, int file_count, t_uls_flags* flags);
bool mx_is_implied_dir(const char* dir_name);
int mx_get_total_blocks(t_file** files);
char* mx_get_file_path(const char* dir_name, const char* file_name);
char* mx_get_file_size(long int f_size);

// Function prototypes for files and file array handling
void mx_handle_file_array(t_file** files, const char* dir_name, bool is_dir, bool is_single, t_uls_flags* flags);
int mx_handle_dir(const char* dir_name, t_uls_flags* flags, bool is_single);

// Function prototypes for printing file information
void mx_output_files_l(t_file** files, bool is_dir, t_uls_flags* flags);
void mx_output_for_F(mode_t mode);
void mx_output_default(t_file** files, t_uls_flags* flags);
void mx_output_for_m(t_file** files, t_uls_flags* flags);
void mx_print_dir_name(const char* dir_name);
void mx_print_color_file_name(char* file_name, mode_t mode);
void mx_output_acl_info(const char* acl_str);
void mx_output_file_xattr(const char* path, bool is_h_on);

// Function prototypes for flag utility functions
bool mx_is_uls_flags_applied(t_uls_flags* flags, char* file_name);
void mx_add_flag(t_uls_flags** flags, char flag);

// Function prototypes for error handling
void mx_print_flag_err(char flag);
void mx_print_dir_error(const char* error, const char* dir_name);
void mx_print_perm_dir_error(const char* error, const char* dir_path, bool is_single);

// Miscellaneous utility functions
void mx_reset_file_params_length(t_file** files, t_uls_flags* flags);

// Function prototypes for linked list operations
void mx_clear_list(t_file** list);
void mx_push_back(t_file** list, const char* dir_name, const char* file_name, t_uls_flags* flags);
int mx_list_size(t_file* list);

// Function prototypes for sorting
void mx_sort_init(t_file** files, t_uls_flags* flags);
void mx_sort_list(t_file** list, bool (*cmp)(t_file* a, t_file* b));
void mx_reverse_list(t_file** list);

// Miscellaneous utility functions
char* mx_dtoa(float d, int precision);
int mx_toupper(int c);
void mx_printerr(const char* s);
char* mx_strrchr(const char* s, int c);

#endif
