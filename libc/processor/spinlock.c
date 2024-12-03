#include "../include/spinlock.h"
#include <stdatomic.h>
#include <stdint.h>

void spinlock_aquire(Spinlock *lock) {
  while (atomic_flag_test_and_set(&lock->flag)) {
    __builtin_ia32_pause();
  }
}

void spinlock_release(Spinlock *lock) { atomic_flag_clear(&lock->flag); }
