#ifndef EXCEPTION_HANDLERS_H
#define EXCEPTION_HANDLERS_H
#include <stdint.h>
#include <stdnoreturn.h>
typedef struct {

  uint32_t int_no;

  uint32_t gs;
  uint32_t fs;
  uint32_t es;
  uint32_t ds;

  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t dummy_esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  uint32_t error_code; // if any
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
} isr_frame_t;

__attribute__((noreturn)) void
error_code_exception_handler(isr_frame_t *stack_frame);
__attribute__((noreturn)) void
no_error_code_exception_handler(isr_frame_t *stack_frame);

#endif
