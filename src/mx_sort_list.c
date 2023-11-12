#include "../inc/uls.h"

// Helper function to merge two sorted linked lists based on the provided comparison function
static t_file* merge(t_file* left, t_file* right, bool (*cmp)(t_file* a, t_file* b)) {
    t_file* result = NULL;

    // Base cases: if one of the lists is empty, return the other list
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    // Compare elements and merge the lists accordingly
    if (cmp(left, right)) {
        result = left;
        result->next = merge(left->next, right, cmp);
    } else {
        result = right;
        result->next = merge(left, right->next, cmp);
    }

    return result;
}

// Helper function to split a linked list into two halves
static void split(t_file* source, t_file** front, t_file** back) {
    t_file* fast;
    t_file* slow;

    // Base cases: if the list is empty or has only one element
    if (source == NULL || source->next == NULL) {
        *front = source;
        *back = NULL;
    } else {
        // Use fast and slow pointers to find the midpoint
        slow = source;
        fast = source->next;

        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        // Split the list into two halves
        *front = source;
        *back = slow->next;
        slow->next = NULL;
    }
}

// Function to perform merge sort on a linked list based on the provided comparison function
void mx_sort_list(t_file** list, bool (*cmp)(t_file* a, t_file* b)) {
    t_file* head = *list;
    t_file* left;
    t_file* right;

    // Base case: if the list is empty or has only one element, it is already sorted
    if (head == NULL || head->next == NULL) {
        return;
    }

    // Split the list into two halves
    split(head, &left, &right);

    // Recursively sort the two halves
    mx_sort_list(&left, cmp);
    mx_sort_list(&right, cmp);

    // Merge the sorted halves
    *list = merge(left, right, cmp);
}
