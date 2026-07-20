#include "task.h"
#include "kheap.h"
#include "terminal.h"

extern "C" void switch_task_asm(uint32_t* old_esp, uint32_t new_esp);

task_t* current_task = 0;
task_t* ready_queue = 0;
uint32_t next_pid = 1;

void init_tasking() {
    asm volatile("cli");

    //create the kernel thread 
    current_task = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = 0;
    current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_dir = 0; 
    
    current_task->next = current_task; 
    ready_queue = current_task;
    
    terminal_write("Multitasking Initialized. Main thread active.\n");
    asm volatile("sti");
}

void switch_task_cpp() {
    if (!current_task) return;

    task_t* old_task = current_task;
    current_task = current_task->next;
    
    if (old_task == current_task) 
        return;
    
    switch_task_asm(&old_task->esp, current_task->esp);
}

// Function to
void thread_exit() {
    terminal_write("Thread finished!\n");
    while(1) 
    { 
        asm volatile("cli; hlt"); }
}

void create_thread(void (*thread_function)()) {
    asm volatile("cli");
    
    task_t* new_task = (task_t*)kmalloc(sizeof(task_t));
    new_task->id = next_pid++;
    
    uint32_t* stack = (uint32_t*)kmalloc(4096);
    
    uint32_t* esp = (uint32_t*)((uint32_t)stack + 4096);
    
    
    *(--esp) = (uint32_t)thread_exit;      
    *(--esp) = (uint32_t)thread_function; 
    *(--esp) = 0;
    *(--esp) = 0;
    *(--esp) = 0;
    *(--esp) = 0;
    
    new_task->esp = (uint32_t)esp;
    new_task->ebp = 0;
    new_task->eip = (uint32_t)thread_function;
    new_task->page_dir = current_task->page_dir;
    
   // circular linked list
    new_task->next = ready_queue->next;
    ready_queue->next = new_task;
    
    asm volatile("sti");
}
