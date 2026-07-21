#include "shell.h"
#include "terminal.h"

bool strcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}

void execute_command(const char* command) {
    terminal_write("\n");
    
    if (command[0] == '\0') {
    }
    else if (strcmp(command, "help")) {
        terminal_write("Available commands:\n");
        terminal_write("  help    - Show this message\n");
        terminal_write("  clear   - Clear the screen\n");
        terminal_write("  panic   - Test the blue screen of death\n");
    }
    else if (strcmp(command, "clear")) {
        terminal_initialize();
    }
    else if (strcmp(command, "panic")) {
        terminal_write("SYSTEM PANIC INITIATED BY USER.\n");
        while(1) { asm volatile("cli; hlt"); }
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
    }
    
    terminal_write("bastOS> ");
}
