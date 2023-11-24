#include "libmx.h"

// Function to count the number of digits in an integer
static int mx_digit_count(int number) {
    int counter = 0;

    // Count digits by continuously dividing the number by 10 until it becomes 0
    do {
        number /= 10;
        ++counter;
    } while (number != 0);

    return counter; // Return the count of digits
}

// Function to convert an integer to a string
char *mx_itoa(int number) {
    // Handling the special case of the minimum int value (-2147483648)
    if (number == -2147483648) {
        return mx_strdup("-2147483648");
    }

    // Allocate memory for the resulting string based on the digit count of the number
    char *result = mx_strnew(mx_digit_count(number));
    int sign;
    
    // Handle the sign of the number
    if ((sign = number) < 0) {
        number = -number;
    }

    int i = 0;
    // Extract digits from the number and store them in the string
    do {
        result[i++] = number % 10 + 48; // Convert digit to character representation
    
    } while ((number /= 10) > 0);
    
    // Add '-' sign if the number was negative
    if (sign < 0) {
        result[i++] = '-';
    }
    
    result[i] = '\0'; // Add null terminator to mark the end of the string

    mx_str_reverse(result); // Reverse the string to get the correct order of digits
    
    return result; // Return the resulting string representation of the number
}
