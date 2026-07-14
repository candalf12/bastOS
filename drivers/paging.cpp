#include "paging.h"
#include "pmm.h"
#include "terminal.h"

page_directory_t* kernel_directory = 0;

void map_page(uint32_t physical_address, uint32_t virtual_address, uint32_t flags) {
    // Calculate which Page Directory Entry (PDE) and Page Table Entry (PTE) this address falls into
    uint32_t pd_index = virtual_address >> 22;
    uint32_t pt_index = (virtual_address >> 12) & 0x03FF;

    if (kernel_directory->tables[pd_index] == 0) {
        //ask PMM to create to create a new frame for Page
        uint32_t new_table_phys = (uint32_t)pmm_alloc_frame();
        
        //create a pointer.
        kernel_directory->tables[pd_index] = (page_table_t*)new_table_phys;
        
        kernel_directory->entries[pd_index] = new_table_phys | PAGE_PRESENT | PAGE_RW;

        for (int i = 0; i < 1024; i++) {
            kernel_directory->tables[pd_index]->entries[i] = 0;
        }
    }

    kernel_directory->tables[pd_index]->entries[pt_index] = physical_address | flags;
}

void init_paging() {
    kernel_directory = (page_directory_t*)pmm_alloc_frame();

    for (int i = 0; i < 1024; i++) {
        kernel_directory->entries[i] = 0;
        kernel_directory->tables[i] = 0;
    }

    //allocate first 4 MB of ram
    // We map 0x00000000 to 0x00400000.
    for (uint32_t i = 0; i < 0x400000; i += PMM_FRAME_SIZE) {
        //kernel only
        map_page(i, i, PAGE_PRESENT | PAGE_RW);
    }

    terminal_write("Paging tables created and Identity Mapped.\n");
    
   
}
