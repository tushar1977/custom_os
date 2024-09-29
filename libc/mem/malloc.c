#include "../include/memory.h"
#include <stddef.h>
#include <stdio.h>
char *brk = NULL;
char *endp = NULL;
void *base = NULL;

meta_ptr find_block(meta_ptr *last, size_t size) {
  meta_ptr b = base;

  while (b && (b->free && b->size >= size)) {
    *last = b;
    b = b->next;
  }

  return *last;
}
void *sbrk(size_t size) {
  if (size == 0) {
    return (void *)brk;
  }

  void *free = (void *)brk;

  brk += size;
  if (brk >= endp) {
    return NULL;
  }

  return free;
};
void split(meta_ptr block, size_t size) {
  meta_ptr newblock;
  newblock = (struct meta_block *)(block->data + size);
  newblock->size = block->size - size - META_BLOCK_SIZE;
  newblock->next = block->next;
  newblock->free = 1;
  newblock->ptr = newblock->data;
  newblock->prev = block;
  block->next = newblock;
  if (newblock->next) {
    newblock->next->prev = newblock;
  }
}

meta_ptr extend_heap(meta_ptr last, size_t size) {
  meta_ptr old_break, new_break;
  old_break = sbrk(0);
  new_break = sbrk(META_BLOCK_SIZE + size);
  if (new_break == (void *)-1) {
    return NULL;
  }
  old_break->size = size;
  old_break->free = 0;
  old_break->next = NULL;
  old_break->prev = NULL;
  old_break->ptr = old_break->data;
  if (last) {
    last->next = old_break;
  }
  return (old_break);
}

void *malloc(size_t size) {

  meta_ptr block, last;

  size_t s;
  s = align4(size);

  if (base) {
    last = base;
    block = find_block(&last, size);
    if (block) {
      if (block->size - s >= (META_BLOCK_SIZE + 4)) {
        split(block, s);
      }

      block->free = 0;
    } else {
      block = extend_heap(last, s);
      if (!block) {
        return NULL;
      }
    }
  } else {
    block = extend_heap(NULL, s);
    if (!block) {
      return NULL;
    }
    base = block;
  }

  return block->data;
}
