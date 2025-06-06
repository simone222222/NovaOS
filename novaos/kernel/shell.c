// kernel/shell.c
#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "utils.h" // For strcmp, strcpy
#include "io.h"    // For outb (reboot)
#include "fs.h"
#include "string.h" // For strncmp

#define CMD_BUFFER_SIZE 256
char cmd_buffer[CMD_BUFFER_SIZE];

void shell_init(void) {
    vga_write_string("NovaOS Shell v0.1\n");
    vga_write_string("Type 'help' for commands.\n");
}

void print_prompt(void) {
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_write_string("NovaOS> ");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void process_command(char* command) {
    if (strlen(command) == 0) {
        return;
    }

    if (strcmp(command, "help") == 0) {
        vga_write_string("Available commands:\n");
        vga_write_string("  help   - Show this help message\n");
        vga_write_string("  echo   - Echo arguments (e.g., echo hello world)\n");
        vga_write_string("  clear  - Clear the screen\n");
        vga_write_string("  reboot - Reboot the system (via triple fault)\n");
        vga_write_string("  color  - Test VGA colors\n");
        vga_write_string("  ls     - List directory contents\n");
        vga_write_string("  cat    - Display file contents\n");
        vga_write_string("  mkdir  - Create a new directory\n");
        vga_write_string("  cd     - Change current directory\n");
        vga_write_string("  mkfile - Create a new file\n");
    } else if (strncmp(command, "echo ", 5) == 0) {
        vga_write_string(command + 5); // Print everything after "echo "
        vga_newline();
    } else if (strcmp(command, "clear") == 0) {
        vga_clear_screen();
    } else if (strcmp(command, "ls") == 0) {
        fs_list_directory("/");
    } else if (strncmp(command, "cat ", 4) == 0) {
        char buffer[MAX_FILE_SIZE];
        int result = fs_read_file(command + 4, buffer, MAX_FILE_SIZE);
        if (result < 0) {
            vga_write_string("Error: File not found\n");
        } else {
            vga_write_string(buffer);
            vga_newline();
        }
    } else if (strcmp(command, "reboot") == 0) {
        vga_write_string("Rebooting...\n");
        // This is a common way to trigger a reboot on x86.
        // It tells the keyboard controller to pulse the reset line.
        // Or, a more reliable way for QEMU is often a triple fault.
        // outb(0x64, 0xFE); // Command to CPU reset line via keyboard controller
        
        // Let's try a triple fault (often causes reboot in emulators/some hardware)
        // This is done by loading an invalid IDT (Interrupt Descriptor Table)
        // and then causing an interrupt.
        struct {
            uint16_t limit;
            uint32_t base;
        } __attribute__((packed)) idtr;

        idtr.limit = 0;
        idtr.base = 0;
        asm volatile ("lidt (%0)" : : "r"(&idtr)); // Load null IDT
        asm volatile ("int $0x0"); // Trigger interrupt 0 (divide by zero)
                                   // With a null IDT, this causes a double fault.
                                   // If the double fault handler is also bad/null, a triple fault occurs.
                                   // A triple fault usually causes a system reset.

        // If the above doesn't work, just halt
        vga_write_string("Reboot failed. Halting.\n");
        asm volatile("cli; hlt");

    } else if (strcmp(command, "color") == 0) {
        vga_write_string("Testing colors:\n");
        for (int fg = 0; fg < 16; fg++) {
            for (int bg = 0; bg < 16; bg++) {
                if (fg == bg) continue; // Skip same fg/bg
                vga_set_color((enum vga_color)fg, (enum vga_color)bg);
                vga_put_char('X');
            }
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK); // Reset to default for newline
            vga_put_char(' '); // Space between lines of colors
        }
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        vga_newline();

    } else if (strncmp(command, "mkdir ", 6) == 0) {
        int result = fs_create_directory(command + 6);
        if (result == -1) {
            vga_write_string("Error: Filesystem full\n");
        } else if (result == -2) {
            vga_write_string("Error: Directory already exists\n");
        } else {
            vga_write_string("Directory created successfully\n");
        }
    } else if (strncmp(command, "cd ", 3) == 0) {
        int result = fs_change_directory(command + 3);
        if (result == -1) {
            vga_write_string("Error: Directory not found\n");
        } else {
            vga_write_string("Changed directory to: ");
            vga_write_string(fs_get_current_directory());
            vga_newline();
        }
    } else if (strncmp(command, "mkfile ", 7) == 0) {
        // Find the first space after the filename to separate it from content
        char* content_start = command + 7;
        while (*content_start && *content_start != ' ') {
            content_start++;
        }
        
        if (*content_start == ' ') {
            *content_start = '\0';  // Temporarily terminate the filename
            int result = fs_create_file(command + 7, content_start + 1);
            if (result == -1) {
                vga_write_string("Error: Filesystem full\n");
            } else if (result == -2) {
                vga_write_string("Error: File already exists\n");
            } else {
                vga_write_string("File created successfully\n");
            }
        } else {
            vga_write_string("Error: Please provide file content\n");
            vga_write_string("Usage: mkfile filename content\n");
        }
    } else {
        vga_write_string("Unknown command: ");
        vga_write_string(command);
        vga_newline();
    }
}

void shell_run(void) {
    char c;
    int buffer_idx = 0;

    while (1) {
        print_prompt();
        buffer_idx = 0;
        memset(cmd_buffer, 0, CMD_BUFFER_SIZE);

        while (1) {
            c = kb_getchar();
            if (c == '\n') {
                vga_put_char(c); // Echo newline
                cmd_buffer[buffer_idx] = '\0'; // Null-terminate
                process_command(cmd_buffer);
                break; // Break from inner loop to show prompt again
            } else if (c == '\b') { // Backspace
                if (buffer_idx > 0) {
                    buffer_idx--;
                    vga_put_char(c); // Let VGA handle backspace display
                }
            } else if (buffer_idx < CMD_BUFFER_SIZE - 1) {
                cmd_buffer[buffer_idx++] = c;
                vga_put_char(c); // Echo character
            }
        }
    }
}