#include <kernel/exception_handlers.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void print_frame(isr_frame_t *stack_frame) {
  printf("Error code: %i\n", stack_frame->error_code);
  printf("Intterrupt: %i\n", stack_frame->int_no);
  printf("GS: %i\n", stack_frame->gs);
  printf("FS: %i\n", stack_frame->fs);
  printf("ES: %i\n", stack_frame->es);
  printf("DS: %i\n", stack_frame->ds);
  printf("EDI: %i\n", stack_frame->edi);
  printf("ESI: %i\n", stack_frame->esi);
  printf("EBP: %i\n", stack_frame->ebp);
  printf("ESP: %i\n", stack_frame->dummy_esp);
  printf("EBX: %i\n", stack_frame->ebx);
  printf("EDX: %i\n", stack_frame->edx);
  printf("ECX: %i\n", stack_frame->ecx);
  printf("EAX: %i\n", stack_frame->eax);
  printf("EIP: %i\n", stack_frame->eip);
  printf("CS: %i\n", stack_frame->cs);
  printf("EFLAGS: %i\n", stack_frame->eflags);
}

void print_fram_hex(isr_frame_t *stack_frame) {
  printf("Stack base: %u\n", (uint32_t)stack_frame);
  printf("Expected ESP: %u\n", (uint32_t)stack_frame + 68);

  uint32_t *stack = (uint32_t *)stack_frame;
  for (int i = 0; i < 20; i++) {
    printf("[ESP + %u]: %u\n", i * 4, stack[i]);
  }
}

void error_code_exception_handler(isr_frame_t *stack_frame) {
  printf("Exception with err code: %i\nError code: %i\n", stack_frame->int_no,
         stack_frame->error_code);
  while (true) {
    __asm__ volatile("cli; hlt");
  }
}

void no_error_code_exception_handler(isr_frame_t *stack_frame) {
  printf("Exception without err code: %i\n", stack_frame->int_no);
  while (true) {
    __asm__ volatile("cli; hlt");
  }
}
