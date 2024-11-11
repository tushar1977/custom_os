#include "include/gdt.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/multiboot.h"
#include "include/paging.h"
#include "include/serial.h"
#include "include/tty.h"
#include "include/vfs.h"
#include <stdio.h>
void kernel_main(uint32_t magic, struct multiboot_info *bootInfo) {
  terminal_initialize();

  initGdt();
  InitIdt();

  initKeyboard();

  uint32_t mod1 = *(uint32_t *)(bootInfo->mods_addr + 4);
  uint32_t physicsStart = (mod1 + 0xFFF) & ~0xFFF;

  initMemory(bootInfo->mem_upper * 1024, physicsStart);
  VFS *vfs;
  init_vfs(vfs);
  printf("VFS init!\n");
  create_file(vfs, "tushar", "this is my first file");
  printf("file done!\n");
  printf("%s\n", read_file(vfs, "tushar"));
  printf("######################################\n");
  printf("######### WELCOME TO MY OS ###########\n");
  printf("######################################\n");
  printf("tusos--> ");
  for (;;)
    ;
}
