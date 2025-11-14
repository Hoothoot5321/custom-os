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
