#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>

#define HEAP_MAGIC 0xC001CAFE // "Cool Cafe"//


struct block_header_t {
    uint32_t size;
    bool is_free;
    block_header_t* next;
    uint32_t magic;
};

void init_kheap();
void* kmalloc(uint32_t size);
void kfree(void* ptr);

#endif
