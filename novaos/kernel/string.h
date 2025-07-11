#ifndef STRING_H
#define STRING_H

#include <stddef.h>

// String manipulation functions
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int val, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strcat(char* dest, const char* src);

#endif // STRING_H 