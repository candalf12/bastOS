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
    
uint32_t* dynamic_array = (uint32_t*)kmalloc(100 * sizeof(uint32_t));
    
    if (dynamic_array) {
        // Let's write some data to this dynamically allocated memory
        dynamic_array[0] = 0xCAFEBABE;
        dynamic_array[99] = 0xDEADBEEF;
        
        terminal_write("kmalloc SUCCESS: Memory allocated and written to!\n");
    }
    while(1)
    {
        asm volatile("hlt");
    }
    
   
}

