#include "terminal.h"

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Welcome to bastOs\n");
    terminal_write("The OS is now modular.");

}

