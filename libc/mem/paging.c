#include "../include/multiboot.h"
#include "../include/stdio.h"
#include "stdint.h"

void initMemory(struct multiboot_info *bootInfo) {
  unsigned int i = 0;

  while (i < bootInfo->mmap_length) {
    struct multiboot_mmap_entry *mmmt =
        (struct multiboot_mmap_entry *)(bootInfo->mmap_addr + i);

    printf("Low add: %x | High add: %x | Low length: %x | High Length: %x | "
           "size: %x | Type: %x\n",
           mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high,
           mmmt->size, mmmt->type);

    // Move the index by the size of the mmap entry (which is mmmt->size + 4
    // bytes for the size field itself)
    i += mmmt->size + sizeof(mmmt->size);
  }
}
