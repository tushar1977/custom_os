#include "include/gdt.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/kprint.h"
#include "include/memory.h"
#include "include/multiboot.h"
#include "include/paging.h"
#include "include/serial.h"
#include "include/stdio.h"
#include "include/syscall.h"
#include "include/tty.h"
#include "include/vfs.h"
#include "include/vga.h"

void kernel_main(uint32_t magic, struct multiboot_info *bootInfo) {
  terminal_initialize();
  kstatusf("Terminal initialized..\n");

  kstatusf("Initializing GDT..\n");
  initGdt();

  kstatusf("Initializing IDT..\n");
  InitIdt();

  kstatusf("Initializing Syscalls..\n");
  syscall_init();

  kstatusf("Initializing Keyboard..\n");
  initKeyboard();

  uint32_t mod1 = *(uint32_t *)(bootInfo->mods_addr + 4);
  uint32_t physicsStart = (mod1 + 0xFFF) & ~0xFFF;
  kstatusf("Initializing memory with upper mem: %d KB, starting physical "
           "address: %x\n",
           bootInfo->mem_upper, physicsStart);
  initMemory(bootInfo->mem_upper * 1024, physicsStart);
  VFS *vfs;
  init_vfs(vfs);
  kstatusf("Virtual File System Initialized..\n");

  kstatusf("Creating file: tuhsar.txt..\n");
  create_file(vfs, "tuhsar.txt", "fjfjf");

  kstatusf("Creating file: tuhsar2.txt..\n");
  create_file(vfs, "tuhsar2.txt", "fjfjf");

  printf("tusos--> ");

  for (;;) {
  }
}
