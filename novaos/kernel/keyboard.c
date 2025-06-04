// kernel/keyboard.c
#include "keyboard.h"
#include "io.h"
#include "vga.h" // For debug, optional

// PS/2 Controller Ports
#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64

// Basic US QWERTY scancode map (Set 1) - Only printable ASCII and Enter/Backspace
// Does not handle Shift, Ctrl, Alt, CapsLock, etc. for simplicity
const char scancode_to_ascii[] = {
    0,  0/*ESC*/, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b'/*Backspace*/,
    0/*Tab*/, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n'/*Enter*/,
    0/*LCtrl*/, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0/*LShift*/, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0/*RShift*/,
    0/*KP_Multiply*/, 0/*LAlt*/, ' '/*Space*/
    // ... many more keys not mapped for simplicity
};
// For shifted characters (very basic)
const char scancode_to_ascii_shifted[] = {
    0,  0/*ESC*/, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b'/*Backspace*/,
    0/*Tab*/, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n'/*Enter*/,
    0/*LCtrl*/, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0/*LShift*/, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0/*RShift*/,
    0/*KP_Multiply*/, 0/*LAlt*/, ' '/*Space*/
};


#define SCANCODE_LSHIFT 0x2A
#define SCANCODE_RSHIFT 0x36
#define SCANCODE_LSHIFT_RELEASE (SCANCODE_LSHIFT | 0x80)
#define SCANCODE_RSHIFT_RELEASE (SCANCODE_RSHIFT | 0x80)

static int shift_pressed = 0;

void kb_init(void) {
    // For polled keyboard, not much initialization is needed from our side.
    // The BIOS usually sets up the keyboard controller.
    // We could send commands to enable scanning, set LEDs, etc., but
    // for basic input, just reading scancodes is often enough.
    shift_pressed = 0;
}

// Waits for a key press and returns the scancode
uint8_t kb_get_scancode(void) {
    uint8_t scancode = 0;
    // Wait until a key is pressed (output buffer full in status register)
    while (!(inb(KBD_STATUS_PORT) & 0x01));
    scancode = inb(KBD_DATA_PORT);
    return scancode;
}


char kb_getchar(void) {
    uint8_t scancode;
    char ascii_char = 0;

    while(1) {
        scancode = kb_get_scancode();

        // Handle Shift press/release
        if (scancode == SCANCODE_LSHIFT || scancode == SCANCODE_RSHIFT) {
            shift_pressed = 1;
            continue; // Don't return a char for shift itself
        } else if (scancode == SCANCODE_LSHIFT_RELEASE || scancode == SCANCODE_RSHIFT_RELEASE) {
            shift_pressed = 0;
            continue; // Don't return a char for shift release
        }

        // Check if it's a key release event (top bit set)
        if (scancode & 0x80) {
            // It's a release, ignore for simple char input
            // unless it's a modifier key we are tracking, which we did above.
            continue;
        }

        // Map scancode to ASCII
        if (scancode < sizeof(scancode_to_ascii) / sizeof(scancode_to_ascii[0])) {
            if (shift_pressed) {
                ascii_char = scancode_to_ascii_shifted[scancode];
            } else {
                ascii_char = scancode_to_ascii[scancode];
            }
        }

        if (ascii_char != 0) { // If it's a printable char or one we handle (like \n, \b)
            return ascii_char;
        }
        // Otherwise, loop again for a valid key
    }
}