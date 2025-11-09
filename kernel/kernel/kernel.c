#include "kernel/idt.h"
#include "kernel/pic.h"
#include "kernel/test_nasm.h"
#include <stdint.h>
#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

struct gdt_entry_t gdt[GDT_ENTRIES];
struct gdt_ptr_t gdt_ptr;
struct gdt_ptr_t second_gdt_ptr;

void kernel_main(void) {

  __asm__ volatile("cli");

  create_descriptor(0, 0, 0, 0);
  create_descriptor(1, 0, 0xFFFFFF, (GDT_CODE_PL0));
  create_descriptor(2, 0, 0xFFFFFF, (GDT_DATA_PL0));
  create_descriptor(3, 0, 0xFFFFFF, (GDT_CODE_PL3));
  create_descriptor(4, 0, 0xFFFFFF, (GDT_DATA_PL3));

  gdt_ptr.limit = sizeof(gdt[0]) * GDT_ENTRIES - 1;
  gdt_ptr.base = (uint32_t)&gdt;

  set_gdt(gdt_ptr.limit, gdt_ptr.base);
  reload_segments();

  idt_init();
  setup_PIC();

  __asm__ volatile("sti");
  bool pressed = false;

  terminal_initialize();
  // test_print();
  while (true) {
    /*
outb(PIC1_COMMAND, PIC_READ_IRR);
uint8_t val = inb(PIC1_COMMAND);
bool cur = val & 0x02;
if (cur & !pressed) {
uint8_t scancode = inb(0x60);
if (!(scancode & 0x80)) {
}
outb(PIC1_COMMAND, PIC_EOI);
}

pressed = cur;
    */
  }
}
