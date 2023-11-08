#include "libmx.h"

int mx_bubble_sort(char **arr, int size) {
    int swaps = 0;
    for (int i = 0; i < size - 1; i++) {
        for(int j = i + 1; j < size; j++)
        {
            if (mx_strcmp(arr[i], arr[j]) > 0)
            {
                char *tempArr;
                tempArr = arr[i];
                arr[i] = arr[j];
                arr[j] = tempArr;
                swaps++;
            }
        }
    }
    return swaps;
}
