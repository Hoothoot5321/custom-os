#include "kernel/globals.h"
#include "kernel/idt.h"
#include "kernel/pic.h"
#include <kernel/multiboot.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

struct gdt_entry_t gdt[GDT_ENTRIES];
struct gdt_ptr_t gdt_ptr;
struct gdt_ptr_t second_gdt_ptr;

int main(multiboot_info_t *mbd, unsigned int magic) {
  mbd = (multiboot_info_t *)((uint32_t)mbd + VIRTUAL_OFFSET);

  terminal_initialize();
  __asm__ volatile("cli");
  printf("Terminal start\n");

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("Invalid magic!\n");
    return -1;
  }
  if (!(mbd->flags >> 6 & 0x01)) {
    printf("Invalid mmap given by grub\n");
    return -1;
  }

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

  setup_memory_bitmap(mbd);

  __asm__ volatile("sti");

  printf("Start kernel: %x\n", (uint32_t)_kernel_start_phys);
  printf("End kernel: %x\n", (uint32_t)_kernel_end_viz);

  for (size_t index = 0; index < BITMAP_SIZE; index++) {
    uint64_t addr = index * PAGE_SIZE;

    printf("%llx: %i\n", addr, (memory_bitmap[index / 8] >> (index % 8)) & 1);

    if (index % 8 == 0) {
      lock_door();
      door();
      terminal_initialize();
      for (size_t page_index = 0; page_index < mbd->mmap_length;
           page_index += sizeof(multiboot_memory_map_t)) {

        multiboot_memory_map_t *mmm =
            (multiboot_memory_map_t *)(mbd->mmap_addr + page_index +
                                       VIRTUAL_OFFSET);

        printf("Addr: %llx | End Addr: %llx | Type: %i\n", mmm->addr,
               mmm->addr + mmm->len, mmm->type);
      }
    }
  }
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
