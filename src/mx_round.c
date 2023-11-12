#include "../inc/uls.h"

// Function to round a floating-point number to a specified precision
float mx_round(float num, int precision) {
    float multiplier = 1.0;

    // Calculate the multiplier for the desired precision
    for (int i = 0; i < precision; i++) {
        multiplier *= 10.0;
    }

    // Multiply the number by the multiplier to shift the decimal places
    float rounded_num = num * multiplier;

    // Perform rounding based on the sign of the original number
    if (num < 0.0) {
        // For negative numbers, round down (subtract 0.5) and divide by the multiplier
        return (int)(rounded_num - 0.5) / multiplier;
    } else {
        // For non-negative numbers, round up (add 0.5) and divide by the multiplier
        return (int)(rounded_num + 0.5) / multiplier;
    }
}
