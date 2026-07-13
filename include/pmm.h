#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include "multiboot.h"

#define PMM_FRAME_SIZE 4096 

void init_pmm(multiboot_info* mbi);

void pmm_init_region(uint32_t base_addr, uint32_t size);
void pmm_deinit_region(uint32_t base_addr, uint32_t size);
void* pmm_alloc_frame();
void pmm_free_frame(void* frame);

#endif
