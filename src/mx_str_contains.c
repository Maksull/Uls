#include "../inc/uls.h"

// Function to check if a character is present in a string
bool mx_str_contains(char *str, char c) {
    // Check if the string is NULL
    if (str == NULL) {
        return false;
    }

    // Iterate through the string to find the character
    for (int i = 0; str[i] != '\0'; i++) {
        // If the character is found, return true
        if (str[i] == c) {
            return true;
        }
    }

    // If the character is not found, return false
    return false;
}
