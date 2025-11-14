#ifndef PIC_HANDLER_H
#define PIC_HANDLER_H

#include <stddef.h>
#include <stdint.h>
void pic_handler(uint32_t interrupt_number);
void wait(size_t seconds);

#endif
