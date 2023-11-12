#include "../inc/uls.h"

// Function to set front spaces to a string to achieve a specified length
static void set_front_spaces(char** str, int max_length) 
{
    int difference = max_length - mx_strlen(*str);

    char spaces[difference + 1];
    mx_memset(spaces, ' ', difference);
    spaces[difference] = '\0';

    mx_strcat(spaces, *str);

    mx_strdel(str);
    *str = mx_strdup(spaces);
}

// Function to set back spaces to a string to achieve a specified length
static void set_back_spaces(char** str, int max_length) 
{
    int difference = max_length - mx_strlen(*str);

    char spaces[difference + 1];
    mx_memset(spaces, ' ', difference);
    spaces[difference] = '\0';

    char tmp[mx_strlen(*str) + difference + 1];

    mx_strcpy(tmp, *str);
    mx_strcat(tmp, spaces);

    mx_strdel(str);
    *str = mx_strdup(tmp);
}

// Function to reset the length of the 'num_links' field in file structures
static void reset_file_num_links_length(t_file** files)
{
    int max_length = 0;
    int len;

    // Finding the maximum length of 'num_links' among all files
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->num_links);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    // Setting front spaces to 'num_links' to achieve the maximum length
    while (file)
    {
        if (mx_strlen(file->num_links) < max_length)
        {
            set_front_spaces(&file->num_links, max_length);
        }
        file = file->next;
    }
}

// Function to reset the length of the 'size' field in file structures
static void reset_file_size_length(t_file** files, t_uls_flags* flags)
{
    int max_length = flags->h ? 4 : 0;
    int len;

    // Finding the maximum length of 'size' among all files
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->size);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    // Setting front spaces to 'size' to achieve the maximum length
    while (file)
    {
        if (mx_strlen(file->size) < max_length)
        {
            set_front_spaces(&file->size, max_length);
        }
        file = file->next;
    }
}

// Function to reset the length of the 'group_name' field in file structures
static void reset_file_group_name_length(t_file** files)
{
    int max_length = 0;
    int len;

    // Finding the maximum length of 'group_name' among all files
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->group_name);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    // Setting back spaces to 'group_name' to achieve the maximum length
    while (file)
    {
        if (mx_strlen(file->group_name) < max_length)
        {
            set_back_spaces(&file->group_name, max_length);
        }
        file = file->next;
    }
}

// Function to reset the length of the 'user_name' field in file structures
static void reset_file_user_name_length(t_file** files)
{
    int max_length = 0;
    int len;

    // Finding the maximum length of 'user_name' among all files
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->user_name);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    // Setting back spaces to 'user_name' to achieve the maximum length
    while (file)
    {
        if (mx_strlen(file->user_name) < max_length)
        {
            set_back_spaces(&file->user_name, max_length);
        }
        file = file->next;
    }
}

// Function to reset the length of the 'lm_day' field in file structures
static void reset_file_lm_day_length(t_file** files)
{
    int max_length = 2;
    int len;

    // Finding the maximum length of 'lm_day' among all files
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->lm_day);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    // Setting front spaces to 'lm_day' to achieve the maximum length
    while (file)
    {
        if (mx_strlen(file->lm_day) < max_length)
        {
            set_front_spaces(&file->lm_day, max_length);
        }
        file = file->next;
    }
}

// Function to reset the length of the 'lm_time' field in file structures
static void reset_file_lm_time_length(t_file** files)
{
    int max_length = 5;
    int len;

    // Finding the maximum length of 'lm_time' among all files
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->lm_time);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    // Setting front spaces to 'lm_time' to achieve the maximum length
    while (file)
    {
        if (mx_strlen(file->lm_time) < max_length)
        {
            set_front_spaces(&file->lm_time, max_length);
        }
        file = file->next;
    }
}

// Function to reset the length of various parameters in file structures
void mx_reset_file_params_length(t_file** files, t_uls_flags* flags) 
{
    // Resetting the length of 'num_links', 'user_name', 'group_name', 'size', 'lm_day', and 'lm_time'
    reset_file_num_links_length(files);
    reset_file_user_name_length(files);
    reset_file_group_name_length(files);
    reset_file_size_length(files, flags);
    reset_file_lm_day_length(files);
    reset_file_lm_time_length(files);
}
