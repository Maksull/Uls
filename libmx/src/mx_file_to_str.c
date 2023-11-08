#include "libmx.h"

char *mx_file_to_str(const char *file)
{
    int f = open(file, O_RDONLY);
    if(f == -1){
        close(f);
        return NULL;
    }

    int len = 0;
    char c;
    int check = read(f, &c, 1);
    while (check > 0) {
        check = read(f, &c, 1);
        len++;
    }
    close(f);

    f = open(file, O_RDONLY);

    char *result = mx_strnew(len);
    read(f, result, len);
    close(f);
    
    return result;
}
