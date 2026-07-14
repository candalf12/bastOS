#include "kheap.h"
#include "paging.h"
#include "pmm.h"
#include "terminal.h"
//start where we left of.
uint32_t placement_address = 0x400000;

uint32_t heap_mapped_up_to = 0x400000;

void* kmalloc(uint32_t size) {
    if (placement_address % 4 != 0) {
        placement_address += 4 - (placement_address % 4);
    }

    uint32_t tmp = placement_address;
    
    placement_address += size;

    while (placement_address > heap_mapped_up_to) {
        uint32_t phys_frame = (uint32_t)pmm_alloc_frame();
        
        if (phys_frame == 0) {
            terminal_write("PANIC: kmalloc out of physical memory!\n");
            return 0;
        }

        map_page(phys_frame, heap_mapped_up_to, PAGE_PRESENT | PAGE_RW);
        
        heap_mapped_up_to += PMM_FRAME_SIZE; // (4096)
    }

    return (void*)tmp;
}
