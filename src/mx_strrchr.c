#include "../inc/uls.h"

// Custom implementation of the strrchr function
char *mx_strrchr(const char *s, int c) {
    // Check if the input string is NULL
    if (s == NULL) {
        return NULL;
    }

    // Get the length of the input string
    int len = mx_strlen(s);

    // Iterate through the string in reverse order
    for (int i = len; i >= 0; i--) {
        // If the character is found, return a pointer to it
        if (s[i] == c) {
            return (char *)&(s[i]);
        }
    }

    // If the character is not found, return NULL
    return NULL;
}
