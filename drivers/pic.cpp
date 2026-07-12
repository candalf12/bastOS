#include "pic.h"
#include "io.h"

//pic hardware ports on the motherboard
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x10 
#define ICW1_ICW4    0x01

#define ICW4_8086    0x01 

void pic_remap(uint32_t offset1, uint32_t offset2) {
    uint8_t a1, a2;

    // saving the current interrupt masks. 
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // initilazation is started
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    //for the master 0x20 is passed and 0x28 for the slave (32, 40 in decimal) 
    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20); 
    }
    outb(PIC1_COMMAND, 0x20);    
}
