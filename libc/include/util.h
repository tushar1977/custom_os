#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define CEIL_DIV(a, b) (((a + b) - 1) / b)

void outPortB(uint16_t port, uint8_t value);

char inPortB(uint16_t port);
struct InterruptRegisters {
  uint32_t cr2;
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, csm, eflags, useresp, ss;
};

#endif
