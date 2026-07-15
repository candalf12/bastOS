#include "isr.h"
#include "terminal.h"
#include "paging.h"
#include "pmm.h"

extern "C" void load_page_directory(uint32_t*);
extern page_directory_t* kernel_directory;
// An array of Intel's official exception messages
const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

extern "C" void isr_handler(registers_t* regs) {
     // Intercept Page Fault
    if (regs->int_no == 14) {  
        uint32_t faulting_address; //error code is in cr2
        asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
        // Analyze the error code
        int present = !(regs->err_code & 0x1);
        if(present)
        {
            uint32_t page_aligned_addr = faulting_address & 0xFFFFF000;
            
            uint32_t phys_frame = (uint32_t)pmm_alloc_frame();
            
            if (phys_frame != 0) {
                map_page(phys_frame, page_aligned_addr, PAGE_PRESENT | PAGE_RW);
                asm volatile("invlpg (%0)" ::"r" (page_aligned_addr) : "memory");

                return;
            }
        }
        int rw = regs->err_code & 0x2;         // Write operation?
        int us = regs->err_code & 0x4;         // Processor was in user-mode?
        int reserved = regs->err_code & 0x8;   //overwrite error. 
        // Print the panic message
        terminal_write("PAGE FAULT ( ");
        if (present) terminal_write("present ");
        if (rw) terminal_write("read-only ");
        if (us) terminal_write("user-mode ");
        if (reserved) terminal_write("reserved ");
        terminal_write(") at ");
        
        terminal_write_hex(faulting_address);
        terminal_write("\n");
        
        terminal_write("System Halted.\n");
        while(1) { 
            asm volatile("cli; hlt"); 
        }
    }
    terminal_write("CPU exception is encountered\n");

    if (regs->int_no < 32) {
        terminal_write("Exception: ");
        terminal_write(exception_messages[regs->int_no]);
        terminal_write("\n");
    }

    terminal_write("System Halted.\n");

    while(1) {
        asm volatile("cli; hlt");
    }
}
