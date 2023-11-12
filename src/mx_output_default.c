#include "../inc/uls.h"

// Function to get the file at a specific index in the linked list
static t_file* get_file_at_index(t_file* file, int index) {
    // Iterate through the linked list until reaching the desired index
    int i = 0;
    while (file) {
        if (i == index)
        {
            return file;
        }
        
        file = file->next;
        ++i;
    }

    return NULL;
}

// Function to calculate the maximum length of file names in the list
static int get_max_name_length(t_file** files)
{
    int max_name_len = 0;
    int len;

    // Traverse the linked list to find the maximum length of file names
    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->name);
        if (len > max_name_len)
        {
            max_name_len = len;
        }
        file = file->next;
    }

    return max_name_len;
}

// Function to calculate the column width based on file names
static int calc_col_width(t_file** files)
{
    int col_width = get_max_name_length(files);

    // Adjust column width to the next multiple of TAB_SIZE
    if (col_width % TAB_SIZE == 0) {
        col_width += TAB_SIZE;
    }
    else {
        col_width += TAB_SIZE - (col_width % TAB_SIZE);
    }
    
    return col_width;
}

// Function to calculate the number of tabs needed based on column width and name length
static int calc_num_tabs(int col_width, int name_len)
{
    int difference = col_width - name_len;

    // Calculate the number of tabs required to align the next column
    if (difference % TAB_SIZE != 0) {
        return difference / TAB_SIZE + 1;
    } else { 
        return difference / TAB_SIZE;
    }
}

// Function to print a specific number of tabs based on column width and name length
static void print_tabs(int col_width, int name_len)
{
    int num_tabs = calc_num_tabs(col_width, name_len);
    
    // Print the calculated number of tabs
    for (int i = 0; i < num_tabs; i++)
    {
        mx_printchar('\t');
    }
}

// Function to calculate the number of columns based on window columns and column width
static int calc_cols(int win_cols, int col_width)
{
    int cols = (win_cols / col_width) == 0 ? 1 : win_cols / col_width;

    // Adjust the number of columns to fit within the window width
    if (col_width * cols > win_cols && cols != 1) {
        cols--;
    }

    return cols;
}

// Function to calculate the number of rows based on columns and number of names
static int calc_rows(int cols, int num_names)
{
    int rows = num_names / cols;

    // Adjust the number of rows to accommodate all names
    if (rows == 0 || num_names % cols != 0) {
        rows++;
    }

    return rows;
}

// Function to print rows of file names based on calculated rows and columns
static void print_rows(t_file** files, int col_width, int rows, int num_names, t_uls_flags* flags)
{
    // Iterate through rows and columns to print the file names
    for (int i = 0; i < rows; i++) {

        for (int j = 0; i + j < num_names; j += rows) {

            t_file* curr_file = get_file_at_index(*files, i + j);
            // Print file name with color if -G flag is set and the output is a terminal
            if (flags->G && isatty(1))
            {
                mx_print_color_file_name(curr_file->name, curr_file->stat.st_mode);
            }
            else {
                mx_printstr(curr_file->name);
            }

            // Append special characters based on file type if -F flag is set
            flags->F ? mx_output_for_F(curr_file->stat.st_mode) : (void) 0;

            // Print tabs to align the next column
            if (curr_file->next && (i + j + rows < num_names)) 
            {
                print_tabs(col_width, mx_strlen(curr_file->name));
            }
        }
        // Move to the next line after printing each row
        mx_printchar('\n');
    }
}

// Function to print a single row of file names
static void print_single_row(t_file** files, int col_width, t_uls_flags* flags)
{
    t_file* file = *files;
    // Iterate through the linked list to print file names in a single row
    while (file)
    {
        // Print file name with color if -G flag is set and the output is a terminal
        if (flags->G && isatty(1))
        {
            mx_print_color_file_name(file->name, file->stat.st_mode);
        }
        else {
            mx_printstr(file->name);
        }
        // Append special characters based on file type if -F flag is set
        flags->F ? mx_output_for_F(file->stat.st_mode) : (void) 0;
        
        // Print tabs to align the next column
        if (file->next)
        {
            print_tabs(col_width, mx_strlen(file->name));
        }
        // Move to the next file in the linked list
        file = file->next;
    }
    // Move to the next line after printing the single row
    mx_printchar('\n');
}

// Function to print file names based on window columns and flags
static void print_file_names(t_file** files, int col_width, int win_cols, t_uls_flags* flags)
{
    int cols = calc_cols(win_cols, col_width);
    int rows;
    int num_names = mx_list_size(*files);

    // Decide whether to print in rows or a single row based on the number of names and window width
    if (num_names * col_width > win_cols) {
        rows = calc_rows(cols, num_names);
        print_rows(files, col_width, rows, num_names, flags);

    } else if (num_names != 0) {
        print_single_row(files, col_width, flags);
    }
}

// Main function to output file names in the default format
void mx_output_default(t_file** files, t_uls_flags* flags)
{
    struct winsize win_size;

    // Calculate the column width for formatting
    int col_width = calc_col_width(files);
    // Get the window size for determining the number of columns
    ioctl(1, TIOCGWINSZ, &win_size);

    // Check terminal conditions and output file names accordingly
    if ((!isatty(1) || flags->one) && !flags->C) {
        // If not a terminal or -1 flag is set, print names in a single column
        int max_len = get_max_name_length(files);
        print_file_names(files, col_width, max_len, flags);
    }
    else if (isatty(1) && !flags->one && !flags->C)
    {
        // If a terminal and -1 flag is not set, print names based on window width
        print_file_names(files, col_width, win_size.ws_col, flags);
    }
    else {
        // If -C flag is set, print names in multiple columns with a fixed width
        print_file_names(files, col_width, CAT_WIDTH, flags);
    }
}
