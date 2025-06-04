// kernel/keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void kb_init(void); // Placeholder, not much to do for polled
char kb_getchar(void); // Blocking, gets a character

#endif // KEYBOARD_H