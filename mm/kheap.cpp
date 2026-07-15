#include "kheap.h"
#include "paging.h"
#include "pmm.h"
#include "terminal.h"
//start where we left of.
uint32_t current_heap_end = 0x400000;

block_header_t* free_list_head = 0;

void init_kheap() {
    free_list_head = 0;
}

void* kmalloc(uint32_t size) {
    if (size == 0) return 0;
    
    // free block in linked list architecture
    block_header_t* current = free_list_head;
    block_header_t* previous = 0;
    
    while (current != 0) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;

            return (void*)((uint32_t)current + sizeof(block_header_t));
        }
        previous = current;
        current = current->next;
    }
   
    //no free found we expand the heap.
    //after demand paging, do not need to call map_page().
    
    block_header_t* new_block = (block_header_t*)current_heap_end;
    current_heap_end += (sizeof(block_header_t) + size);
    
    new_block->size = size;
    new_block->is_free = false;
    new_block->magic = HEAP_MAGIC;
    new_block->next = 0;
    
    if (previous != 0) {
        previous->next = new_block;
    } else {
        free_list_head = new_block;
    }
    
    return (void*)((uint32_t)new_block + sizeof(block_header_t));
}
void kfree(void* ptr) {
    if (ptr == 0) return;
    
    //looking behind to find hidden header. 
    block_header_t* block = (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));
    
    if (block->magic != HEAP_MAGIC) {
        terminal_write("kfree() called on invalid pointer or corrupted heap!\n");
        while(1) { 
            asm volatile("cli; hlt"); 
        }
    }
    
    // Free the block!
    block->is_free = true;
}
