#include "kernel/globals.h"
#include <kernel/pic.h>
#include <kernel/pic_handler.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static size_t last_clock_counter = 0;

void pic_handler(uint32_t interrupt_number) {
  // EOI is handled by NASM pic_asm.S
  // printf("Got interrupt: %u\n", interrupt_number);
  if (interrupt_number == 0) {
    system_timer_counter += 1;
    if (((system_timer_counter - last_clock_counter) % (18 * 5)) == 0) {
      printf("%u seconds has passed\n", 5);
      last_clock_counter = system_timer_counter;
    }
  }
  if (interrupt_number == 1) {
    uint8_t key_code = inb(PERIPHERAL_PORT);
    if (!(key_code & 0x80)) {
      printf("Key pressed: %u\n", key_code);
    }
  }
}
