// kernel/vga.c
#include "vga.h"
#include "io.h" // For outb/inb if needed for cursor, but we'll keep it simple

static size_t vga_row;
static size_t vga_column;
static uint8_t vga_color_attr;
static uint16_t* const vga_buffer = VGA_MEMORY;

static inline uint8_t make_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t make_vga_entry(char c, uint8_t color) {
    return (uint16_t) c | (uint16_t) color << 8;
}

void vga_update_cursor() {
    unsigned short position = (vga_row * VGA_WIDTH) + vga_column;
    outb(0x3D4, 0x0F); // Tell VGA we are setting the low cursor byte
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E); // Tell VGA we are setting the high cursor byte
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void vga_scroll() {
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            const size_t next_line_index = (y + 1) * VGA_WIDTH + x;
            vga_buffer[index] = vga_buffer[next_line_index];
        }
    }
    // Clear the last line
    uint16_t blank_entry = make_vga_entry(' ', vga_color_attr);
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank_entry;
    }
    vga_row = VGA_HEIGHT - 1;
}

void vga_init(void) {
    vga_row = 0;
    vga_column = 0;
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear_screen();
}

void vga_set_color(enum vga_color fg, enum vga_color bg) {
    vga_color_attr = make_color(fg, bg);
}

void vga_put_char_at(char c, uint8_t color, size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    const size_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = make_vga_entry(c, color);
}

void vga_newline() {
    vga_column = 0;
    vga_row++;
    if (vga_row >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_backspace() {
    if (vga_column > 0) {
        vga_column--;
        vga_put_char_at(' ', vga_color_attr, vga_column, vga_row);
    } else if (vga_row > 0) {
        // This is a simple backspace, doesn't wrap lines upwards.
        // For a real shell, you'd want to move up and to the end of the previous line.
        // For now, just go to previous column if possible, or stay at (0,0)
        vga_column = 0; // or VGA_WIDTH -1 if you want to go to end of prev line
    }
    vga_update_cursor();
}


void vga_put_char(char c) {
    if (c == '\n') {
        vga_newline();
    } else if (c == '\b') { // Handle backspace
        vga_backspace();
    } else {
        vga_put_char_at(c, vga_color_attr, vga_column, vga_row);
        vga_column++;
        if (vga_column >= VGA_WIDTH) {
            vga_newline();
        }
    }
    vga_update_cursor();
}

void vga_write_string(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++)
        vga_put_char(data[i]);
}

void vga_clear_screen(void) {
    uint16_t blank_entry = make_vga_entry(' ', vga_color_attr);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = blank_entry;
        }
    }
    vga_row = 0;
    vga_column = 0;
    vga_update_cursor();
}