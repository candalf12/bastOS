#include "pmm.h"
#include "terminal.h"

extern uint32_t end;

uint32_t* memory_bitmap = 0;
uint32_t max_blocks = 0;
uint32_t used_blocks = 0;

// Sets a bit to 1 (Marks frame as USED)
inline void mmap_set(uint32_t bit) {
    memory_bitmap[bit / 32] |= (1 << (bit % 32));
}

// Sets a bit to 0 (Marks frame as FREE)
inline void mmap_unset(uint32_t bit) {
    memory_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

// Checks if a bit is 1 or 0
inline bool mmap_test(uint32_t bit) {
    return memory_bitmap[bit / 32] & (1 << (bit % 32));
}

void pmm_init_region(uint32_t base_addr, uint32_t size) {
    uint32_t align = base_addr / PMM_FRAME_SIZE;
    uint32_t frames = size / PMM_FRAME_SIZE;
    for (; frames > 0; frames--) {
        mmap_unset(align++);
        used_blocks--;
    }
}
int mmap_first_free() {
    uint32_t bitmap_size = max_blocks / 32;
    
    for (uint32_t i = 0; i < bitmap_size; i++) {
        if (memory_bitmap[i] != 0xFFFFFFFF) {
            
            for (int j = 0; j < 32; j++) {
                int bit = 1 << j;
                if (!(memory_bitmap[i] & bit)) {
                    // returning the index where it is free
                    return i * 32 + j;
                }
            }
        }
    }
    
    return -1;
}
void init_pmm(multiboot_info* mbi) {
    uint32_t kernel_end = (uint32_t)&end;
    
    memory_bitmap = (uint32_t*)kernel_end;

    uint32_t memory_size_kb = mbi->mem_upper + 1024;
    uint32_t memory_size_bytes = memory_size_kb * 1024;
    
    max_blocks = memory_size_bytes / PMM_FRAME_SIZE;
    
    //Default mark 0xFF (used)    
    uint32_t bitmap_size = max_blocks / 32;
    for (uint32_t i = 0; i < bitmap_size; i++) {
        memory_bitmap[i] = 0xFFFFFFFF;
    }
    
    used_blocks = max_blocks;
        used_blocks = max_blocks;
    
    
    multiboot_mmap_entry* mmap = (multiboot_mmap_entry*)mbi->mmap_addr;
    
    while ((uint32_t)mmap < mbi->mmap_addr + mbi->mmap_length) {
        
        //type 1 means free ram
        if (mmap->type == 1) {
            pmm_init_region(mmap->addr_low, mmap->len_low);
        }
        
        // moving the pointer to the next entry.
        mmap = (multiboot_mmap_entry*) ( (uint32_t)mmap + mmap->size + sizeof(mmap->size) );
    }

   //to protect the kernel. in frame-wise.  
    uint32_t reserved_size = kernel_end + (bitmap_size * 4); 
    uint32_t reserved_frames = reserved_size / PMM_FRAME_SIZE;
    if (reserved_size % PMM_FRAME_SIZE) reserved_frames++;
    
    // Mark these frames as USED (1)
    for (uint32_t i = 0; i < reserved_frames; i++) {
        mmap_set(i);
        used_blocks++;
    }

    terminal_write("PMM: Memory map parsed. Usable RAM is now free!\n");

}
//4KB of memory allocation. 
void* pmm_alloc_frame() {
    //if OOM occurs 
    if (used_blocks >= max_blocks) {
        terminal_write("Out of Physical Memory!\n");
        return 0;
    }

    int frame_idx = mmap_first_free();

    if (frame_idx == -1) {
        terminal_write("Out of Physical Memory!\n");
        return 0; // Return NULL
    }

    //marking the frame as used 
    mmap_set(frame_idx);
    used_blocks++;

    //Calculate the physical address
    uint32_t physical_address = frame_idx * PMM_FRAME_SIZE;
    
    return (void*)physical_address;
}

void pmm_free_frame(void* frame) {
    uint32_t physical_address = (uint32_t)frame;
    
    int frame_idx = physical_address / PMM_FRAME_SIZE;

    mmap_unset(frame_idx);
    used_blocks--;
}
