#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"  
#include "timer.h"

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Welcome to bastOS\n");
    terminal_write("The OS is now modular.\n");
    init_gdt();
    terminal_write("GDT is loaded. Ring 3 structure is based.\n");
    init_idt();
    terminal_write("IDT is loaded. Interrupts prepared.\n");
    
    pic_remap(0x20,0x28);
    terminal_write("PIC reprogrammed.\n");
    init_timer(1000);
    terminal_write("Timer initialized at 1000Hz.\n");
    asm volatile("sti");
    terminal_write("Keystroke is being waited.\n");

    while(1)
    {
        asm volatile("hlt");
    }
    
   
}

