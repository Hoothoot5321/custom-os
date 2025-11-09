
#include <kernel/test_nasm.h>
#include <stdio.h>

void test_print() {
  uint32_t val = nasm_print(7, 2);
  printf("Out: %u\n", val);
}
