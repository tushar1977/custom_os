#include "../include/syscall.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/vfs.h"
#include "stdio.h"
void *syscall_table[NUM_SYSCALLS] = {(void *)create_file, (void *)printf};
void syscall_dispatcher(struct InterruptRegisters *regs) {
  if (regs->eax >= NUM_SYSCALLS) {
    printf("Invalid syscall number: %d\n", regs->eax);
    regs->eax = -1;
    return;
  }

  void *system_api = syscall_table[regs->eax];
  int ret;

  printf("Syscall %d called with args: %x, %x, %x, %x, %x\n", regs->eax,
         regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

  asm volatile("pushl %%ebx\n"    // Save caller's EBX
               "movl %2, %%ebx\n" // Load syscall arg1
               "pushl %%edi\n"    // Save caller's EDI
               "movl %3, %%edi\n" // Load syscall arg2
               "pushl %%esi\n"    // Save caller's ESI
               "movl %4, %%esi\n" // Load syscall arg3
               "call *%1\n"       // Call syscall function
               "popl %%esi\n"     // Restore caller's ESI
               "popl %%edi\n"     // Restore caller's EDI
               "popl %%ebx\n"     // Restore caller's EBX
               : "=a"(ret)        // Return value in EAX
               : "r"(system_api), // Function pointer
                 "r"(regs->ebx),  // First argument
                 "r"(regs->edi),  // Second argument
                 "r"(regs->esi)   // Third argument
               : "memory", "cc"   // Clobber memory and condition codes
  );

  regs->eax = ret;
}

void syscall_init() {
  printf("Initializing syscall handler...\n");
  irq_install_handler(0x80, syscall_dispatcher);
  printf("Syscall handler installed at interrupt 0x80\n");
}
