#include "gdt.h"
tss_entry_t tss_entry;
uint8_t kernel_stack[4096];
extern "C" void gdt_flush(uint32_t);

gdt_entry_t gdt_entries[6];
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
void write_tss(int32_t num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    //adding tss to gdt
    gdt_set_gate(num, base, limit, 0xE9, 0x00);


    __builtin_memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0  = ss0;  // Set the kernel stack segment
    tss_entry.esp0 = esp0; // Set the kernel stack pointer
    
    tss_entry.iomap_base = sizeof(tss_entry);
}
void init_gdt() {
    //Set up the GDT pointer (Size of table minus 1, and the memory address)
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
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
    write_tss(5, 0x10, (uint32_t)&kernel_stack + 4096); 

    gdt_flush((uint32_t)&gdt_ptr);
    tss_flush();
}
