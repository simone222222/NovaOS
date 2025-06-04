// kernel/utils.c
#include "utils.h"

// Basic itoa for positive numbers
char* itoa(int value, char* str, int base) {
    char* ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (base < 2 || base > 36) { *str = '\0'; return str; }
    if (value == 0) { *ptr++ = '0'; *ptr = '\0'; return str; }

    // Negative numbers not handled yet for simplicity, or make them positive
    if (value < 0 && base == 10) {
        // value = -value; // Handle negative if you want
        // *ptr++ = '-';
    }
    
    while (value != 0) {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    }
    
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return str;
}