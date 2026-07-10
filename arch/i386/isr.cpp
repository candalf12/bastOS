#include "isr.h"
#include "terminal.h"

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

extern "C" void isr_handler(registers_t regs) {
    terminal_write("CPU exception is encountered\n");

    if (regs.int_no < 32) {
        terminal_write("Exception: ");
        terminal_write(exception_messages[regs.int_no]);
        terminal_write("\n");
    }

    terminal_write("System Halted.\n");

    while(1) {
        asm volatile("cli; hlt");
    }
}
