#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_remap(uint32_t offset1, uint32_t offset2);
void pic_send_eoi(uint8_t irq);

#endif
