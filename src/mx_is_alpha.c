#include "../inc/uls.h"

// Checks if the character is an alphabetical letter (either uppercase or lowercase)
bool mx_is_alpha(char c) {
    // Checks if the character falls within the ASCII range of uppercase or lowercase letters
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
        return true; // Returns true if it's an alphabetical character
    }

    return false; // Returns false if it's not an alphabetical character
}
