#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdatomic.h>
typedef struct {
  atomic_flag flag;
} Spinlock;

void spinlock_aquire(Spinlock *);
void spinlock_release(Spinlock *);

#endif // !SPINLOCK_H
