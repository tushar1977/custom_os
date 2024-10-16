#include "../include/tty.h"
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool print(const char *data, size_t length) {
  const unsigned char *bytes = (const unsigned char *)data;
  for (size_t i = 0; i < length; i++)
    if (putchar(bytes[i]) == EOF)
      return false;
  return true;
}

static void print_int(int v) {
  char buffer[12];
  int i = 0;

  if (v == 0) {
    buffer[i++] = '0';
  }

  if (v < 0) {
    buffer[i++] = '-';
    v = -v;
  }

  while (v > 0) {
    buffer[i++] = (v % 10) + '0';
    v /= 10;
  }

  for (int j = 0; j < i / 2; j++) {
    char temp = buffer[j];
    buffer[j] = buffer[i - j - 1];
    buffer[i - j - 1] = temp;
  }

  buffer[i] = '\0';
  print(buffer, i);
}

void itoa_hex(int value, char *buffer) {
  const char *hex_digits = "0123456789ABCDEF";
  int i = 0;
  unsigned int temp = (unsigned int)value;

  if (value == 0) {
    buffer[i++] = '0';
    buffer[i] = '\0';
    return;
  }

  while (temp != 0) {
    buffer[i++] = hex_digits[temp % 16];
    temp /= 16;
  }

  buffer[i] = '\0';

  // Reverse the string to get correct hexadecimal order
  for (int j = 0; j < i / 2; ++j) {
    char tmp = buffer[j];
    buffer[j] = buffer[i - j - 1];
    buffer[i - j - 1] = tmp;
  }
}
int printf(const char *restrict format, ...) {
  va_list parameters;
  va_start(parameters, format);
  char buffer[32];

  int written = 0;

  while (*format != '\0') {
    size_t maxrem = INT_MAX - written;

    if (format[0] != '%' || format[1] == '%') {
      if (format[0] == '%')
        format++;
      size_t amount = 1;
      while (format[amount] && format[amount] != '%')
        amount++;
      if (maxrem < amount) {

        return -1;
      }
      if (!print(format, amount))
        return -1;
      format += amount;
      written += amount;
      continue;
    }

    const char *format_begun_at = format++;

    switch (*format) {
    case 'c': {
      format++;
      char c = (char)va_arg(parameters, int /* char promotes to int */);
      if (!maxrem) {
        return -1;
      }
      if (!print(&c, sizeof(c)))
        return -1;
      written++;
      break;
    }
    case 's': {
      format++;
      const char *str = va_arg(parameters, const char *);
      size_t len = strlen(str);
      if (maxrem < len) {
        return -1;
      }
      if (!print(str, len))
        return -1;
      written += len;
      break;
    }
    case 'd': {
      format++;

      int c = (int)va_arg(parameters, int);
      print_int(c);
      written += sizeof(int);
      break;
    }

    case '\n': {
      newline();
      written++;
      break;
    }

    case '\b': {
      format++;
      backspace();
      if (written > 0) {
        written--;
      }
      break;
    }

    case 'x': {
      format++;
      int num = va_arg(parameters, int);
      itoa_hex(num, buffer);
      size_t len = strlen(buffer);
      if (maxrem < len) {
        return -1;
      }
      if (!print(buffer, len))
        return -1;
      written += len;
      break;
    }

    default: {
      format = format_begun_at;
      size_t len = strlen(format);
      if (maxrem < len) {
        return -1;
      }
      if (!print(format, len))
        return -1;
      written += len;
      format += len;
      break;
    }
    }
  }

  va_end(parameters);
  return written;
}
