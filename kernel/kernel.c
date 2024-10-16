#include "include/gdt.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/multiboot.h"
#include "include/paging.h"
#include "include/stdio.h"
#include "include/tty.h"
void kernel_main(uint32_t magic, struct multiboot_info *bootInfo) {
  terminal_initialize();

  initGdt();
  InitIdt();

  initKeyboard();

  for (;;)
    ;
}
