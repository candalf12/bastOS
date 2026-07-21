#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

bool strcmp(const char* str1, const char* str2);

void execute_command(const char* command);

#endif
