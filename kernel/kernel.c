#include "include/kernel/tty.h"
#include "include/keyboard/keypress.h"
#include "include/memory/memory.h"
#include "include/stdio/stdio.h"

void kernel_main(void) {
  terminal_initialize();

  unsigned char keyboard_buffer[256];

  unsigned int i = 0;

  for (i = 0; i < 256; i++) {
    keyboard_buffer[i] = '\0';
  }

  int *p = (int *)malloc(3 * sizeof(int));

  *p = 20;

  printf("%d", *p);

  free(p);

  i = 0;

  while (1) {
    unsigned char scan_code = 0;

    while (scan_code == 0) {
      scan_code = get_key_press();
    }

    unsigned char ascii_char = keycode_to_ascii(scan_code);

    if (ascii_char != 0) {
      keyboard_buffer[i] = ascii_char;
      i++;
      putchar(ascii_char);
    }

    while (get_key_press() == scan_code) {
    }
  }
}
