#include "../include/paging.h"

#include "../include/multiboot.h"
#include "../include/string.h"
#include "../include/util.h"
#include "stdint.h"
#include "stdio.h"
#include <stddef.h>
uint8_t physicalMemoryBitmap[NUM_PAGE_FRAMS / 8];

static uint32_t pageDirs[NUM_PAGE_DIRS][1024] __attribute__((aligned(4096)));
static uint8_t pageDirsUsed[NUM_PAGE_DIRS];

void initMemory(uint32_t memHigh, uint32_t physicalStart) {
  initial_page_dir[0] = 0;
  invalidate(0);

  initial_page_dir[1023] = ((uint32_t)initial_page_dir - KERNEL_START) |
                           PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
  invalidate(0xFFFFF000);

  pmm_init(physicalStart, memHigh);

  memset(pageDirs, 0, 0x1000 * NUM_PAGE_DIRS);
  memset(pageDirsUsed, 0, NUM_PAGE_DIRS);
}

void invalidate(uint32_t vadd) { asm volatile("invlpg %0" : : "m"(vadd)); }

void pmm_init(uint32_t memLow, uint32_t memHigh) {

  pageFrameMin = CEIL_DIV(memLow, 0x1000);
  pageFrameMax = memHigh / 0x1000;
  totalAlloc = 0;

  memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
}
