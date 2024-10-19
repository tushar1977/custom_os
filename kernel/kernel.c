#include "include/gdt.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/multiboot.h"
#include "include/paging.h"
#include "include/tty.h"
#include <stdio.h>

void kernel_main(uint32_t magic, struct multiboot_info *bootInfo) {
  terminal_initialize();

  initGdt();
  printf("GDT initialized!\n");
  InitIdt();
  printf("IDT initialized!\n");

  initKeyboard();

  uint32_t mod1 = *(uint32_t *)(bootInfo->mods_addr + 4);
  uint32_t physicsStart = (mod1 + 0xFFF) & ~0xFFF;

  initMemory(bootInfo->mem_upper * 1024, physicsStart);
  printf("Memory allocation done\n");
  terminal_writestring("Paging is done!\n");
  for (;;)
    ;
}
