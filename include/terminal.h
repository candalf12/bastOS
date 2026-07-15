#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize();
void terminal_putchar(char c);
void terminal_write(const char* data);
void terminal_write_hex(uint32_t n);

#endif
