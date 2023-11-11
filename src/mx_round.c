#include "../inc/uls.h"

float mx_round(float num, int precision) {
    float multiplier = 1.0;

    for (int i = 0; i < precision; i++) {
        multiplier *= 10.0;
    }

    float rounded_num = num * multiplier;

    if (num < 0.0) {
        return (int)(rounded_num - 0.5) / multiplier;
    } else {
        return (int)(rounded_num + 0.5) / multiplier;
    }
}
