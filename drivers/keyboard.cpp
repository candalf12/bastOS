#include "keyboard.h"
#include "io.h"
#include "pic.h"
#include "terminal.h"
#include "shell.h"


const char kbd_US[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t',         /* Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`',   0,        /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
    'm', ',', '.', '/',   0,        /* Right shift */
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char shell_buffer[256];
uint8_t shell_index = 0;

extern "C" void keyboard_handler() {
    uint8_t scancode = inb(0x60);

    if (!(scancode & 0x80)) {
        char c = kbd_US[scancode];
        
        if (c == '\n') {
            shell_buffer[shell_index] = '\0';
            execute_command(shell_buffer);
            shell_index = 0;
        } 
        else if (c == '\b') {
            if (shell_index > 0) {
                shell_index--;
                terminal_putchar('\b');
            }
        } 
        else if (c != 0 && shell_index < 255) {
            shell_buffer[shell_index] = c;
            shell_index++;
            char str[2] = {c, '\0'};
            terminal_write(str);
        }
    }
    pic_send_eoi(1);
}
