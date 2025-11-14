#include "kernel/default_interrupts.h"
#include "kernel/pic.h"
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <stdint.h>

__attribute__((aligned(0x10))) idt_entry_t
    idt[256]; // Create an array of IDT entries; aligned for performance
//
bool vectors[IDT_MAX_DESCRIPTORS];
//
void idt_set_descriptor(uint8_t idt_entry_index, uint32_t isr, uint8_t flags) {

  idt_entry_t *descriptor = &idt[idt_entry_index];

  descriptor->isr_low = isr & 0xFFFF;
  descriptor->kernel_cs = 0x08; // this value can be whatever offset your kernel
                                // code selector is in your GDT
  descriptor->attributes = flags;
  descriptor->isr_high = isr >> 16;
  descriptor->reserved = 0;
}

static idtr_t idtr;

void idt_init() {
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

  for (uint8_t vector = 0; vector < CPU_EXCEPTION_COUNT; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector],
                       IDT_DESCRIPTOR_EXCEPTION);
    vectors[vector] = true;
  }
  for (uint8_t vector = CPU_EXCEPTION_COUNT;
       vector < CPU_EXCEPTION_COUNT + PIC_INTERRUPT_COUNT; vector++) {
    idt_set_descriptor(vector, pic_stub_table[vector - CPU_EXCEPTION_COUNT],
                       IDT_DESCRIPTOR_EXTERNAL);
    vectors[vector] = true;
  }

  for (uint8_t vector = 0; vector < IDT_MAX_DESCRIPTORS - 1; vector++) {
    if (!vectors[vector]) {
      idt_set_descriptor(vector, default_stub_table[vector],
                         IDT_DESCRIPTOR_EXTERNAL);
    }
    vectors[vector] = true;
  }
  set_idt(idtr.limit, idtr.base);
}
