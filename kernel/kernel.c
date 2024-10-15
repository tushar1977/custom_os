#include "include/gdt.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/memory.h"
#include "include/stdio.h"
#include "include/tty.h"

void kernel_main(void) {
  terminal_initialize();

  initGdt();
  InitIdt();

  initKeyboard();
  for (;;)
    ;
}
