// kernel/utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h> // For size_t
#include "string.h" // For string functions

void* memset(void* bufptr, int value, size_t size);
void* memcpy(void* dstptr, const void* srcptr, size_t size);
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
// Basic itoa for positive numbers
char* itoa(int value, char* str, int base);


#endif // UTILS_H