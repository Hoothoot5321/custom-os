#include <stdint.h>
#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

struct GDTEntry gdt[GDT_ENTRIES];
struct GDTPtr gdt_ptr;

struct GDTPtr second_gdt_ptr;
void kernel_main(void) {
  __asm__ volatile("cli");

  create_descriptor(0, 0, 0, 0);
  create_descriptor(1, 0, 0xFFFFFF, (GDT_CODE_PL0));
  create_descriptor(2, 0, 0xFFFFFF, (GDT_DATA_PL0));
  create_descriptor(3, 0, 0xFFFFFF, (GDT_CODE_PL3));
  create_descriptor(4, 0, 0xFFFFFF, (GDT_DATA_PL3));

  gdt_ptr.limit = sizeof(gdt[0]) * GDT_ENTRIES - 1;
  gdt_ptr.base = (uint32_t)&gdt;

  setGdt(gdt_ptr.limit, gdt_ptr.base);
  reloadSegments();

  __asm__ volatile("sti");

  terminal_initialize();
  printf("Hello, kernel World!\n");

  int val = 5;

  printf("My name is %u\n", val);

  printf("Old Limit %u\n", gdt_ptr.limit);
  printf("Old Base %u\n", gdt_ptr.base);
  __asm__ volatile("sgdt %0" : "=m"(second_gdt_ptr));

  printf("New Limit %u\n", second_gdt_ptr.limit);
  printf("New Base %u\n", second_gdt_ptr.base);
}
