#include <stdint.h>
#include "terminal.h"

//fenerbahce kusu
#define STACK_CHK_GUARD 0xDEADBEEF 

//gcc looks for this exact variable name to seed the stack
extern "C" uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

extern "C" __attribute__((noreturn)) void __stack_chk_fail(void) {
    terminal_write("Buffer Overflow\n");
    terminal_write("System Halted to prevent code execution.\n");
    while (1) {
        asm volatile("cli; hlt");
    }
}
