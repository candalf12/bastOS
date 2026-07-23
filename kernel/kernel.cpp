#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"  
#include "timer.h"
#include "multiboot.h"
#include "pmm.h"
#include "paging.h"
#include "kheap.h"
#include "task.h"  
#include "pci.h"

extern "C" void kernel_main(uint32_t magic, multiboot_info* mbi) {
    terminal_initialize();
    if(magic != MULTIBOOT_MAGIC){
        terminal_write("invalid multiboot magic number.");
        return;
    }
    terminal_write("Welcome to bastOS\n");
    
    if (mbi->flags & (1 << 6)) {

        init_pmm(mbi);
        init_paging();

    } else {
        terminal_write("No memory map provided!\n");
    }
    
    init_gdt();

    init_idt();

    
    pic_remap(0x20,0x28);
 
    init_timer(1000);
  
    asm volatile("sti");
      
    void* frame1 = pmm_alloc_frame();
    void* frame2 = pmm_alloc_frame();
    
    
    pmm_free_frame(frame1);
    pmm_free_frame(frame2);
   

    init_kheap();
    init_tasking();

    terminal_write("\n\n");
    terminal_write("bastOS> ");
    init_pci();
    enter_user_mode();

    while(1)
    {
        asm volatile("hlt");
    }
}
