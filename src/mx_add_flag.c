#include "../inc/uls.h"

void mx_add_flag(t_uls_flags** flags, char flag) 
{
    switch (flag) {
        case 'l':
            (*flags)->l = 1;
            (*flags)->C = 0;
            (*flags)->m = 0;
            (*flags)->one = 0;
            break;
        case 'a':
            (*flags)->a = 1;
            (*flags)->A = 0;
            break;
        case 'A':
            (*flags)->a = 0;
            (*flags)->A = 1;
            break;
        case 'G':
            (*flags)->G = 1;
            break;
        case 'T':
            (*flags)->T = 1;
            break;
        case 'r':
            (*flags)->r = 1;
            break;
        case 't':
            (*flags)->t = 1;
            break;
        case 'u':
            (*flags)->u = 1;
            (*flags)->c = 0;
            break;
        case 'c':
            (*flags)->c = 1;
            (*flags)->u = 0;
            break;
        case 'S':
            (*flags)->S = 1;
            break;
        case 'R':
            (*flags)->R = 1;
            break;
        case 'h':
            (*flags)->h = 1;
            break;
        case '@':
            (*flags)->at = 1;
            break;
        case 'e':
            (*flags)->e = 1;
            break;
        case '1':
            (*flags)->one = 1;
            (*flags)->l = 0;
            (*flags)->C = 0;
            (*flags)->m = 0;
            break;
        case 'C':
            (*flags)->C = 1;
            (*flags)->l = 0;
            (*flags)->one = 0;
            break;
        case 'F':
            (*flags)->F = 1;
            break;
        case 'm':
            (*flags)->m = 1;
            (*flags)->one = 0;
            (*flags)->l = 0;
            break;
        case 'f':
            (*flags)->f = 1;
            (*flags)->a = 1;
            (*flags)->A = 0;
            break;
        default:
            break;
    }
}
