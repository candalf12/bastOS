#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "paging.h"

struct task_t {
    uint32_t id;               
    uint32_t esp, ebp;         
    uint32_t eip;              
    page_directory_t* page_dir;
    task_t* next;             
};

void init_tasking();

void switch_task_cpp();

void create_thread(void (*thread_function)());

#endif
