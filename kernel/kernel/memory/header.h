
#ifndef CUSTOM_MALLOC_H
#define CUSTOM_MALLOC_H

#include <stdlib.h>

typedef long Align;

#define NALLOC 1024

union header {
  struct {
    union header *ptr;
    unsigned size;
  } s;
  Align x;
};

typedef union header Header;

void *custom_malloc(unsigned nbytes);
void free(void *ap);

#endif /* CUSTOM_MALLOC_H */
