#ifndef PIC_H
#define PIC_H

#include <stdint.h>

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC1_IDT_OFFSET 0x20
#define PIC2_IDT_OFFSET 0x28

#define PIC_EOI 0x20

#define ICW1_ICW4 0x01      /* Indicates that ICW4 will be present */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define ICW1_INIT 0x10      /* Initialization - required! */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */

#define CASCADE_IRQ 2

#define PIC_READ_IRR 0x0a /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR 0x0b /* OCW3 irq service next CMD read */

#define PIC_INTERRUPT_COUNT 16

#define PERIPHERAL_PORT 0x60

#define PIC1_TIMER_IRQ 0x01
#define PIC1_KEYBOARD_IRQ 0x02
#define PIC1_CASCADE_IRQ 0x04
#define PIC1_SERIAL_2_4_IRQ 0x08
#define PIC1_SERIAL_1_3_IRQ 0x10
#define PIC1_OTHER_PERI_IRQ 0x20
#define PIC1_FLOPPY_DISK_IRQ 0x40
#define PIC1_PARA_1_2_IRQ 0x80
#define PIC2_RTC_IRQ 0x01
#define PIC2_MOUSE_IRQ 0x10
#define PIC2_MATH_EXCEPT_IRQ 0x20
#define PIC2_PRIMARY_IDE_IRQ 0x40
#define PIC2_SECONDARY_IDE_IRQ 0x80

#define PIC1_TIMER_IDT_OFFSET 0
#define PIC1_KEYBOARD_IDT_OFFSET 1
#define PIC1_CASCADE_IDT_OFFSET 2
#define PIC1_SERIAL_2_4_IDT_OFFSET 3
#define PIC1_SERIAL_1_3_IDT_OFFSET 4
#define PIC1_OTHER_PERI_IDT_OFFSET 5
#define PIC1_FLOPPY_DISK_IDT_OFFSET 6
#define PIC1_PARA_1_2_IDT_OFFSET 7
#define PIC2_RTC_IDT_OFFSET 8
#define PIC2_MOUSE_IDT_OFFSET 12
#define PIC2_MATH_EXCEPT_IDT_OFFSET 13
#define PIC2_PRIMARY_IDE_IDT_OFFSET 14
#define PIC2_SECONDARY_IDE_IDT_OFFSET 15

void setup_PIC(void);

extern void outb(uint16_t port, uint8_t val);
extern void outw(uint16_t port, uint16_t val);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern void io_wait(void);

extern uint32_t pic_stub_table[PIC_INTERRUPT_COUNT];

#endif
