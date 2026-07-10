#include "terminal.h"
#include "gdt.h"
#include "idt.h"

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Welcome to bastOs\n");
    terminal_write("The OS is now modular.\n");
    init_gdt();
    terminal_write("GDT is loaded. Ring 3 structure is based.\n");
    init_idt();
    terminal_write("IDT is loaded. Interrupts prepared.\n");
    
    terminal_write("Error is loaded for testing.\n");
    asm volatile("int $0x3");
    terminal_write("If visible IDT does not work.\n");
}

