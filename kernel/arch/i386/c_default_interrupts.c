#include <kernel/default_interrupts.h>
#include <stdio.h>

void default_idt_handler(uint16_t interrupt_number) {
  printf("Default interrupt: %u\n", interrupt_number);
}
