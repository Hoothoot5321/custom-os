#include <kernel/gdt.h>
#include <stddef.h>
#include <stdint.h>

void create_descriptor(size_t entry_index, uint32_t base, uint32_t limit,
                       uint16_t flag) {
  struct gdt_entry_t *desc = &gdt[entry_index];
  desc->limit_low = limit & 0x0000FFFF;
  desc->base_low = base & 0x0000FFFF;
  desc->base_mid = (base >> 16) & 0xFF;
  desc->base_high = (base >> 24) & 0xFF;
  desc->access = flag & 0xFF;
  desc->granularity = (limit >> 16) & 0x000F;
  desc->granularity |= (flag >> 8) & 0xF0;
}
