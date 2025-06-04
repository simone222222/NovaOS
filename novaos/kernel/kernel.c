// kernel/kernel.c
#include "multiboot.h"
#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "utils.h" // For memset, if needed directly
#include "fs.h"    // For filesystem functions

// Kernel entry point
void kmain(multiboot_info_t* mbd, unsigned int magic) {
    (void)mbd; // Unused for now

    vga_init(); // Initialize VGA text mode

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
        vga_write_string("Error: Invalid Multiboot magic number!\n");
        vga_write_string("Expected: 0x");
        char hexbuf[9];
        itoa(MULTIBOOT_BOOTLOADER_MAGIC, hexbuf, 16);
        vga_write_string(hexbuf);
        vga_write_string("\nGot:      0x");
        itoa(magic, hexbuf, 16);
        vga_write_string(hexbuf);
        vga_newline();
        goto halt;
    }
    
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_string("NovaOS Kernel Booted Successfully!\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_write_string("Welcome to NovaOS.\n\n");

    // Initialize filesystem
    fs_init();
    
    // Create sample.txt with content
    fs_create_file("sample.txt", "Hello, world!");

    kb_init();    // Initialize keyboard (simple polled)
    shell_init(); // Initialize shell
    shell_run();  // Start shell loop

halt:
    // Should not be reached if shell_run is infinite
    vga_write_string("\nKernel halting.");
    asm volatile ("cli; hlt"); // Halt the CPU
}