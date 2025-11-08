#include <kernel/exception_handlers.h>

void exception_handler() {
  __asm__ volatile("cli; hlt"); // Completely hangs the computer
}
