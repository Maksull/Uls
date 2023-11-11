#include "../inc/uls.h"

static t_file* merge(t_file* left, t_file* right, bool (*cmp)(t_file* a, t_file* b)) {
    t_file* result = NULL;

    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    if (cmp(left, right)) {
        result = left;
        result->next = merge(left->next, right, cmp);
    } else {
        result = right;
        result->next = merge(left, right->next, cmp);
    }

    return result;
}

static void split(t_file* source, t_file** front, t_file** back) {
    t_file* fast;
    t_file* slow;

    if (source == NULL || source->next == NULL) {
        *front = source;
        *back = NULL;
    } else {
        slow = source;
        fast = source->next;

        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        *front = source;
        *back = slow->next;
        slow->next = NULL;
    }
}

void mx_sort_list(t_file** list, bool (*cmp)(t_file* a, t_file* b)) {
    t_file* head = *list;
    t_file* left;
    t_file* right;

    if (head == NULL || head->next == NULL) {
        return;
    }

    split(head, &left, &right);

    mx_sort_list(&left, cmp);
    mx_sort_list(&right, cmp);

    *list = merge(left, right, cmp);
}
