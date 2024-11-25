#include "../include/syscall.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/vfs.h"
#include "stdio.h"
void *syscall_table[NUM_SYSCALLS] = {(void *)create_file, (void *)printf};
void syscall_dispatcher(struct InterruptRegisters *regs) {
  printf("Syscall dispatcher: syscall number %d\n", regs->eax);

  if (regs->eax >= NUM_SYSCALLS) {
    printf("Invalid syscall number: %d\n", regs->eax);
    return;
  }

  void *system_api = syscall_table[regs->eax];
  int ret;

  printf("Calling syscall function at address %p\n", system_api);

  asm volatile(" \
       push %1; \
       push %2; \
       push %3; \
       push %4; \
       push %5; \
       call *%6; \
       pop %%ebx; \
       pop %%ebx; \
       pop %%ebx; \
       pop %%ebx; \
       pop %%ebx; \
       "
               : "=a"(ret)
               : "r"(regs->edi), "r"(regs->esi), "r"(regs->edx), "r"(regs->ecx),
                 "r"(regs->ebx), "r"(system_api));

  printf("Syscall returned: %d\n", ret);
  regs->eax = ret;
}
void syscall_init() { irq_install_handler(0x80, syscall_dispatcher); }
