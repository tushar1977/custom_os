
#include "../include/memory.h"

void *calloc(size_t n, size_t size) {
  size_t *new;
  size_t s, i;

  new = malloc(n * size);

  if (new) {
    s = align4(n * size) << 2;

    for (int i = 0; i < s; i++) {
      new[i] = 0;
    }
  }

  return new;
}
