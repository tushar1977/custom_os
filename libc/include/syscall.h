#ifndef SYSCALL_H
#define SYSCALL_H
#define NUM_SYSCALLS 2
#include "util.h"

enum SyscallNumbers { SYS_CREATE_FILE = 0, SYS_PRINTF = 1 };

extern void *syscall_table[NUM_SYSCALLS];
void syscall_init(void);

void syscall_dispatcher(struct InterruptRegisters *regs);
void _exit();

#endif
