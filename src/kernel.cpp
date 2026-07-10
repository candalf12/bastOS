#include "terminal.h"
#include "gdt.h"

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Welcome to bastOs\n");
    terminal_write("The OS is now modular.");
    init_gdt();
    terminal_write("GDT is loaded. Ring 3 structure is based.");
    
}

