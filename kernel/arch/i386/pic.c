#include <kernel/pic.h>
#include <stdint.h>
#include <stdio.h>

void PIC_remap(int offset1, int offset2) {
  outb(PIC1_COMMAND,
       ICW1_INIT |
           ICW1_ICW4); // starts the initialization sequence (in cascade mode)
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
  io_wait();
  outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
  io_wait();
  outb(PIC1_DATA, 1 << CASCADE_IRQ); // ICW3: tell Master PIC that there is a
                                     // slave PIC at IRQ2
  io_wait();
  outb(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
  io_wait();

  outb(PIC1_DATA,
       ICW4_8086); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();

  // Unmask both PICs.
  outb(PIC1_DATA,
       0xFF ^ (PIC1_TIMER_IRQ | PIC1_CASCADE_IRQ | PIC1_KEYBOARD_IRQ));
  io_wait();
  outb(PIC2_DATA, 0xFF ^ PIC2_PRIMARY_IDE_IRQ);
  io_wait();
  // outb(PIC1_DATA, ~(1 << 1) & ~(1 << 2)); // unmask IRQ1 + cascade
  // outb(PIC2_DATA, 0xFF);                  // mask all slave IRQs
}

void setup_PIC() { PIC_remap(PIC1_IDT_OFFSET, PIC2_IDT_OFFSET); }
