#ifndef GLOBALS_H
#define GLOBALS_H

#include "kernel/multiboot.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_ERROR_LEN 50
#define MAX_LOOP_SECONDS 10

#define PAGE_SIZE 0x1000
#define MAX_PHYS_MEM 0x100000000ULL // 4GB
#define BITMAP_SIZE (MAX_PHYS_MEM / PAGE_SIZE / 8)

#define VIRTUAL_OFFSET 0xC0000000

extern char _kernel_start_phys[];
extern char _kernel_end_viz[];

extern volatile size_t system_timer_counter;

extern char cur_error[MAX_ERROR_LEN + 1];

extern uint8_t memory_bitmap[BITMAP_SIZE];

uint8_t set_bit(uint8_t val, uint8_t index, bool on);

void write_cur_error(char *msg);

void print_cur_error();

void lock_door();

void unlock_door();

void door();
void setup_memory_bitmap(multiboot_info_t *mbd);

#endif
