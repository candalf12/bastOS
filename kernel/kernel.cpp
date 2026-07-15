#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"  
#include "timer.h"
#include "multiboot.h"
#include "pmm.h"
#include "paging.h"
#include "kheap.h"
extern "C" void kernel_main(uint32_t magic, multiboot_info* mbi) {
    terminal_initialize();
    if(magic != MULTIBOOT_MAGIC){
        terminal_write("invalid multiboot magic number.");
        return;
    }
    terminal_write("Welcome to bastOS\n");
    //checking 6th bit.
    if (mbi->flags & (1 << 6)) {
        terminal_write("Memory map provided by GRUB!\n");
        init_pmm(mbi);
        init_paging();
        terminal_write("Paging works.");
        uint32_t total_memory_kb = mbi->mem_upper + 1024;
    } else {
        terminal_write("No memory map provided!\n");
    }
    init_gdt();
    terminal_write("GDT is loaded. Ring 3 structure is based.\n");
    init_idt();
    terminal_write("IDT is loaded. Interrupts prepared.\n");
    
    pic_remap(0x20,0x28);
    terminal_write("PIC reprogrammed.\n");
    init_timer(1000);
    terminal_write("Timer initialized at 1000Hz.\n");
    asm volatile("sti");
      
    void* frame1 = pmm_alloc_frame();
    void* frame2 = pmm_alloc_frame();
    
    if (frame1 && frame2) {
        terminal_write("PMM handed out two physical frames!\n");
    } else {
        terminal_write("PMM failed to allocate memory.\n");
    }
    
    // Free them back to the pool
    pmm_free_frame(frame1);
    pmm_free_frame(frame2);
    terminal_write("PMM: Frames freed.\n");
    
    init_kheap();
    void* ptr1 = kmalloc(100);
    terminal_write_hex((uint32_t)ptr1);
    terminal_write("\n");
    void* ptr2 = kmalloc(200);
    terminal_write_hex((uint32_t)ptr2);
    terminal_write("\n");
    kfree(ptr1);
    void* ptr3 = kmalloc(50);
    terminal_write_hex((uint32_t)ptr3);
    terminal_write("\n");
    if(ptr3 == ptr1)
    {
        terminal_write("ptr3 used the memory for ptr1. Kfree function works.\n");
    }
    terminal_write("Linked list malloc is implemented.");

    volatile uint32_t* bad_ptr = (volatile uint32_t*) 0xA00000;
    *bad_ptr = 0xDEADBEEF;
    if(*bad_ptr == 0xDEADBEEF)
    {
        terminal_write("Paging worked.");
    }
    while(1)
    {
        asm volatile("hlt");
    }
    
   
}

