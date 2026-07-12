#include "timer.h"
#include "io.h"
#include "pic.h"
#include "terminal.h"

uint32_t tick = 0;

extern "C" void timer_handler() {
    tick++;
    // if (tick % 1000 == 0) {
    //     terminal_write(".");
    // }
    pic_send_eoi(0);
}

void init_timer(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    outb(0x40, l);
    outb(0x40, h);
}
