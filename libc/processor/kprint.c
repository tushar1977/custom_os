#include "../include/kprint.h"
#include "../include/spinlock.h"
#include "../include/vga.h"
#include <stdarg.h>
#include <stdio.h>
Spinlock spinlock = {ATOMIC_FLAG_INIT};
void acquire_serial_lock() { spinlock_aquire(&spinlock); }
void kstatusf(const char *format, ...) {
  acquire_serial_lock();

  printf("[STATUS] " __FILE__ ": ");

  va_list args;
  va_start(args, format);
  printf(format, args); // Use vprintf for variadic arguments
  va_end(args);

  spinlock_release(&spinlock);
}

void k_ok() {
  printf("hi\n");
  spinlock_aquire(&spinlock);
  printf("OK!\n");
  spinlock_release(&spinlock);
}
