#include "terminal.h"
#include "io.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
volatile uint16_t* terminal_buffer;

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void terminal_update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = 7 | 0 << 4;
    terminal_buffer = (volatile uint16_t*) 0xB8000;
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_enable_cursor(0,15);
    terminal_update_cursor(0,0);
}
void terminal_scroll() {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
    }
}
void terminal_putchar(char c) {
    if (c == '\b') {
        if(terminal_column > 0)
        {
            terminal_column--;
            size_t index = terminal_row * VGA_WIDTH + terminal_column;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
        terminal_update_cursor(terminal_column, terminal_row);

        return;
    }
    if(c == '\n')
    {
        terminal_column = 0;
        if(terminal_row < VGA_HEIGHT - 1)
        {
            terminal_row++;
        } else {
            terminal_scroll();
        }
        terminal_update_cursor(terminal_column, terminal_row);
        return;
    }

    if (c == 27) {
        return;
    }
    if (terminal_column >= VGA_WIDTH - 1) {
        if (c == ' ') return; 
        size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = vga_entry(c, terminal_color);
        terminal_update_cursor(terminal_column, terminal_row);
        return;
    }
    
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = vga_entry(c, terminal_color);
    terminal_column++;

    if(terminal_column >= VGA_WIDTH)
    {
        terminal_column = 0;
        if(terminal_row < VGA_HEIGHT - 1)
        {
            terminal_row++;
        }
        else
        {
            terminal_scroll();
        }
    }
    terminal_update_cursor(terminal_column, terminal_row);
}
void terminal_write(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        terminal_putchar(data[i]);
    }
    terminal_update_cursor(terminal_column, terminal_row);
}
