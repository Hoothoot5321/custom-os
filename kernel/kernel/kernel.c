#include "kernel/idt.h"
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

  __asm__ volatile("sti");

  terminal_initialize();
  printf("%u, %u\n", 26946, 12345);
  printf("GOING TO ERROR BABY!!\n");
  __asm__ __volatile__(
      "cli\n\t"               // Disable interrupts to avoid noise
      "mov $0x6944, %%ax\n\t" // Invalid segment selector value
      "mov %%ax, %%ds\n\t"    // Load DS with invalid selector, triggers #GP
      "hlt\n\t"               // Halt CPU after fault to stop further execution
      :
      :
      : "ax");

  __asm__ volatile("cli; hlt");

  printf("Old Limit %u\n", gdt_ptr.limit);
  printf("Old Base %u\n", gdt_ptr.base);
  __asm__ volatile("sgdt %0" : "=m"(second_gdt_ptr));

  printf("New Limit %u\n", second_gdt_ptr.limit);
  printf("New Base %u\n", second_gdt_ptr.base);
}
