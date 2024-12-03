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
  // Initialize the terminal
  terminal_initialize();
  kstatusf("Terminal initialized..\n");

  // Initialize GDT
  kstatusf("Initializing GDT..\n");
  initGdt();

  // Initialize IDT
  kstatusf("Initializing IDT..\n");
  InitIdt();

  // Initialize syscall
  kstatusf("Initializing Syscalls..\n");
  syscall_init();

  // Initialize keyboard
  kstatusf("Initializing Keyboard..\n");
  initKeyboard();

  // Memory initialization
  uint32_t mod1 = *(uint32_t *)(bootInfo->mods_addr + 4);
  uint32_t physicsStart = (mod1 + 0xFFF) & ~0xFFF;
  kstatusf("Initializing memory with upper mem: %d KB, starting physical "
           "address: %x\n",
           bootInfo->mem_upper, physicsStart);
  initMemory(bootInfo->mem_upper * 1024, physicsStart);

  // Initialize VFS (Virtual File System)
  VFS *vfs;
  init_vfs(vfs);
  kstatusf("Virtual File System Initialized..\n");

  // File creation
  kstatusf("Creating file: tuhsar.txt..\n");
  create_file(vfs, "tuhsar.txt", "fjfjf");

  kstatusf("Creating file: tuhsar2.txt..\n");
  create_file(vfs, "tuhsar2.txt", "fjfjf");

  // Prompt for the OS shell
  printf("tusos--> ");

  // Infinite loop to keep the kernel running
  for (;;) {
    // You can add more status prints here as needed
  }
}
