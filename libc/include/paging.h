#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

extern uint32_t initial_page_dir[1024];
static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalAlloc;

void invalidate(uint32_t vadd);
void initMemory(uint32_t memHigh, uint32_t physicalStart);

void pmm_init(uint32_t memLow, uint32_t memHigh);
#define KERNEL_START 0xC0000000

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)
#define NUM_PAGE_DIRS 256
#define NUM_PAGE_FRAMS (0x100000000 / 0x1000 / 8)

#endif
