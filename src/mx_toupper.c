#include "../inc/uls.h"

// Convert a lowercase ASCII character to uppercase
int mx_toupper(int c) {
    // Check if the character is a lowercase letter (ASCII values 97 to 122)
    if (c >= 97 && c <= 122) {
        // Convert to uppercase by subtracting the ASCII difference
        return c - 32;
    } else {
        // Return the character unchanged if it is not a lowercase letter
        return c;
    }
}
