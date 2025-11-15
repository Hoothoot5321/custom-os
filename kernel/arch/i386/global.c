#include "kernel/multiboot.h"
#include <kernel/globals.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool locked = false;

volatile size_t system_timer_counter = 0;
char cur_error[MAX_ERROR_LEN + 1] = {'n', 'o', ' ', 'e', 'r',
                                     'r', 'o', 'r', '\0'};

void write_cur_error(char *msg) {

  int msg_len = strlen(msg);
  if (msg_len > MAX_ERROR_LEN) {
    write_cur_error("Error msg too long");
    return;
  }

  for (int i = 0; i < msg_len; i++) {
    cur_error[i] = msg[i];
  }
  cur_error[msg_len] = '\0';
}

void print_cur_error() { printf("%s\n", cur_error); }

void lock_door() { locked = true; }

void unlock_door() { locked = false; }

void door() {
  lock_door();
  while (locked) {
    __asm__ volatile("hlt");
  }
}
uint8_t memory_bitmap[BITMAP_SIZE] = {0};

uint8_t set_bit(uint8_t val, uint8_t index, bool on) {
  uint8_t local_val = 0;
  local_val = (local_val >> index) | 0x1;
  local_val = local_val << index;
  if (on) {
    local_val = val | local_val;
    return local_val;
  }
  local_val = val & ~local_val;
  return local_val;
}
void setup_memory_bitmap(multiboot_info_t *mbd) {

  for (size_t page_index = 0; page_index < mbd->mmap_length;
       page_index += sizeof(multiboot_memory_map_t)) {
    multiboot_memory_map_t *mmmt =
        (multiboot_memory_map_t *)(mbd->mmap_addr + page_index +
                                   VIRTUAL_OFFSET);

    // printf("%llx\n", mmmt);
    if (mmmt->type != MULTIBOOT_MEMORY_AVAILABLE) {
      continue;
    }

    uint64_t start_adress = mmmt->addr;
    uint64_t end_adress = mmmt->addr + mmmt->len;

    if (start_adress % PAGE_SIZE != 0) {
      start_adress += PAGE_SIZE - (start_adress % PAGE_SIZE);
    }
    if (end_adress % PAGE_SIZE != 0) {
      end_adress -= (end_adress % PAGE_SIZE);
    }

    for (uint64_t cur_addr = start_adress; cur_addr < end_adress;
         cur_addr += PAGE_SIZE) {
      if (cur_addr <= (uint32_t)_kernel_end_viz - VIRTUAL_OFFSET &&
          cur_addr >= (uint32_t)_kernel_start_phys) {
        continue;
      }
      size_t index = cur_addr / PAGE_SIZE;
      memory_bitmap[index / 8] =
          set_bit(memory_bitmap[index / 8], index % 8, true);
    }
  }
}
