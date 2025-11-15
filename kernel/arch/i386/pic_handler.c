#include "kernel/ata_ide.h"
#include "kernel/globals.h"
#include <kernel/pic.h>
#include <kernel/pic_handler.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void wait(size_t seconds) {
  size_t start_time = system_timer_counter;

  __asm__ volatile("sti");

  while ((system_timer_counter - start_time) / 18 < seconds) {
    __asm__ volatile("hlt");
  }
}

void pic_handler(uint32_t interrupt_number) {
  // EOI is handled by NASM pic_asm.S
  if (interrupt_number == PIC2_PRIMARY_IDE_IDT_OFFSET) {
  }
  if (interrupt_number == PIC1_TIMER_IDT_OFFSET) {
    system_timer_counter++;
  }
  if (interrupt_number == PIC1_KEYBOARD_IDT_OFFSET) {
    uint8_t key_code = inb(PERIPHERAL_PORT);
    if (!(key_code & 0x80)) {
      unlock_door();
      // printf("Hello. Its me\n");
    }
  }
}
