#pragma once
#include <stdatomic.h>
#define Spinlock atomic_flag

void spinlock_aquire(Spinlock *);
void spinlock_release(Spinlock *);
