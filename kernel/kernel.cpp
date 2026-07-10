#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"  
extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Welcome to bastOs\n");
    terminal_write("The OS is now modular.\n");
    init_gdt();
    terminal_write("GDT is loaded. Ring 3 structure is based.\n");
    init_idt();
    terminal_write("IDT is loaded. Interrupts prepared.\n");
    
    pic_remap(0x20,0x28);
    terminal_write("PIC reprogrammed.");

    while(1)
    {
        asm volatile("hlt");
    }
    
   
}

