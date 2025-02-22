#include "string.h"

char *memchr2(const void *str, char ch) {
  const char *s = (const char *)str;
  while (*s) {
    if (*s == ch) {
      return (char *)s;
    }
    s++;
  }
  return NULL;
}
