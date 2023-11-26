#include "../inc/uls.h"

// Function to print tabs to align columns in the output
void mx_print_tab(int length, int max_length) {
    int counter = 0; // Counter to determine the number of tabs needed
    int p = max_length - length; // Calculate the difference in length between strings

    // Determine the number of tab characters needed for alignment
    if (p % 8 != 0) {
        counter = (p / 8) + 1; // If the difference isn't divisible by 8, add one for the extra tab
    } else {
        counter = p / 8; // If the difference is divisible by 8, use the result directly
    }
    
    // Print the required number of tab characters for alignment
    for (int a = 0; a < counter; a++) {
        mx_printchar('\t'); // Print a tab character
    }
}
