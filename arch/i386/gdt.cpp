#include "gdt.h"

extern "C" void gdt_flush(uint32_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

void init_gdt() {
    //Set up the GDT pointer (Size of table minus 1, and the memory address)
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // The Null Segment (Required by CPU)
    gdt_set_gate(0, 0, 0, 0, 0);                

    // Kernel Code Segment (Access: 0x9A, Granularity: 0xCF)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 

    //Kernel Data Segment (Access: 0x92, Granularity: 0xCF)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 

    //User Code Segment (Access: 0xFA, Granularity: 0xCF)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 

    //User Data Segment (Access: 0xF2, Granularity: 0xCF)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); 

    gdt_flush((uint32_t)&gdt_ptr);
}
