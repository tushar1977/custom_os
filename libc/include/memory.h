#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)
#define META_BLOCK_SIZE 20
typedef struct meta_block *meta_ptr;
struct meta_block {
  int free;
  size_t size;
  meta_ptr next;
  meta_ptr prev;
  void *ptr;
  char data[1];
};
extern char *brk;
extern char *endp;
extern void *base;

void *malloc(size_t size);
void free(void *ptr);

void *sbrk(size_t size);

int cbrk(void *addr);

void *calloc(size_t n, size_t size);

void *realloc(void *ptr, size_t size);
#endif // MEMORY_H
