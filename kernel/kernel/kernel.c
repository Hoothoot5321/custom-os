#include "kernel/globals.h"
#include "kernel/idt.h"
#include "kernel/pic.h"
#include <kernel/multiboot.h>
#include <stdint.h>
#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

struct gdt_entry_t gdt[GDT_ENTRIES];
struct gdt_ptr_t gdt_ptr;
struct gdt_ptr_t second_gdt_ptr;

int main(multiboot_info_t *mbd, unsigned int magic) {

  uint64_t *val = (uint64_t *)(VIRTUAL_OFFSET | 0x0FFFFFFF);

  terminal_initialize();
  printf("Hello son\n");
  printf("GDT: %llx\n", &gdt_ptr);
  // printf("Nice man\n");
  __asm__ volatile("cli");

  /*
terminal_initialize();

if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
printf("Invalid magic!\n");
return -1;
}
if (!(multi_boot_descriptor->flags >> 6 & 0x01)) {
printf("Invalid mmap given by grub\n");
return -1;
}
  */

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
  printf("Nic cuck\n");
  while (true) {

    __asm__ volatile("hlt");
  }
  return 0;
}

void kernel_main(multiboot_info_t *mbd, unsigned int magic) {
  int out = main(mbd, magic);
  if (out < 0) {
    printf("Main exited with fail\n");
  } else {
    printf("Main exited with success\n");
  }
  while (true) {
  }
}
