#include "keyboard.h"
#include "io.h"
#include "pic.h"
#include "terminal.h"

// ASCII translation table.
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

extern "C" void keyboard_handler() {
    // Read the raw scancode from the keyboard hardware port
    uint8_t scancode = inb(0x60);

    // The highest bit (0x80) is set if a key was RELEASED.
    // If it is not set, the key was PRESSED. Let's only print presses.
    if (!(scancode & 0x80)) {
        char c = kbd_US[scancode];
        
        // Convert the single char to a string format for our terminal
        char str[2] = {c, '\0'};
        terminal_write(str);
    }

    //telling the master pic that I got it 
    pic_send_eoi(1);
}
