#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "isr.h"

// A single entry in a Page Table
typedef uint32_t page_table_entry_t;

struct page_table_t {
    page_table_entry_t entries[1024];
};

typedef uint32_t page_directory_entry_t;

// The Page Directory contains 1024 pointers to Page Tables
struct page_directory_t {
    page_directory_entry_t entries[1024];
    
    page_table_t* tables[1024];
};

//hardware flags  
#define PAGE_PRESENT  0x01  // bit 0, is it in the pm
#define PAGE_RW       0x02  // bit 1 read/write or read-only
#define PAGE_USER     0x04  // bit 2 is it ring 3 structure

void init_paging();
void map_page(uint32_t physical_address, uint32_t virtual_address, uint32_t flags);

#endif
