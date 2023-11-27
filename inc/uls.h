#ifndef ULS_H
#define ULS_H

#include "../libmx/inc/libmx.h"

#include <sys/ioctl.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/acl.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>

// Structs for different configurations and file details

typedef enum e_time_type {
    LAST_MODIFICATION,    // default
    LAST_METADATA_CHANGE, // -c
    LAST_ACCESS,          // -u
    LAST_CREATION,        // -U
} t_time_type; // Enum for different time types

typedef enum e_sort_type {
    BY_NAME,  // default
    BY_TIME,  // -t
    BY_SIZE,  // -S
    UNSORTED, // -f
} t_sort_type; // Enum for different sorting types

typedef enum e_output_format{
    ONE_COLUMN,   // -1
    MULTI_COLUMN, // -C
    STREAM,       // -m
    DETAILED,     // -l
} t_output_format; // Enum for different output formats

typedef enum e_ignore_type{
    HIDDEN,        // default
    DOTS,          // -A
    NOT_IGNORED,   // -a
} t_ignore_type; // Enum for different ignore types

typedef struct s_configuration {
    // Configuration flags for various options
    t_time_type time_type;
    t_sort_type sort_type;
    t_output_format format;
    t_ignore_type ignore_type;
    bool use_recursion;                     // -R
    bool use_colors;                        // -G
    bool classify;                          // -F
    bool add_only_slash_to_directories;     // -p
    bool sort_reverse;                      // -r
    bool sort_horizontally;                 // -x
    bool display_human_readable_size;       // -h
    bool display_numeric_IDs;               // -n
    bool display_non_printable_characters;  // -q
    bool acl;                               // -e
    bool full_time_info;                    // -T
    bool extended_attributes;               // -@
    bool hide_owner_information;            // -g
    bool hide_group_information;            // -o
    bool follow_symbolic_links;             // -H
} t_configuration; // Struct for storing configuration options

// Struct for file details

typedef struct s_file_info {
    char *path;
    char *name;
    char *user;
    char *link;
    char *group;
    char **xattr_keys;
    acl_t acl;
    struct stat stat;
    struct timespec timespec;
} t_file_info; // Struct for storing file information

typedef struct s_width {
    int links;
    int user;
    int group;
    int size;
} t_width; // Struct for storing width information for formatting

//Input configuration
t_configuration *mx_parse_configuration(int argc, char *argv[]);

//File handling 
t_file_info *mx_get_file_info(const char *dir, const char *name, t_configuration *configuration);
void mx_free_file_info(t_file_info *file_info);
void mx_free_file_list_info(t_list *files_info);

//Directory handling 
bool mx_sort_print_dirs(t_list *dirs, t_configuration *configuration, bool must_print_names);
bool mx_sort_print_dir(t_file_info *file_info, t_configuration *configuration, bool must_print_name);
blkcnt_t mx_calculate_count_blocks(t_list *files);
bool mx_get_dir_entries(t_list **entries, const char *name, t_configuration *configuration);
bool mx_is_ignored(const char *name, t_ignore_type ignore_type);

//Sorting
void mx_sort_filenames(t_list *filenames, t_sort_type sort_type);
void mx_sort_file_list_info(t_list *files_info, t_sort_type sort_type, bool isReversed);

//Printing
int mx_print_file_info(t_file_info *files_info, t_configuration *configuration);
void mx_print_one_column(t_list *files_info, t_configuration *configuration);
void mx_print_multi_column(t_list *files_info, t_configuration *configuration);
void mx_print_with_info(t_list *files_info, t_configuration *configuration);
void mx_print_stream(t_list *files_info, t_configuration *configuration);
void mx_print_files_info(t_list *files_info, t_configuration *configuration);

#endif
