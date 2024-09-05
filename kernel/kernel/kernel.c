#include <stdio.h>

#include <kernel/tty.h>
unsigned char get_key_press() {
  unsigned char scan_code;
  asm volatile("inb %1, %0" : "=a"(scan_code) : "Nd"(0x60));
  return scan_code;
}

unsigned char keycode_to_ascii(unsigned char scan_code) {
  switch (scan_code) {
  case 0x1E:
    return 'a';
  case 0x30:
    return 'b';
  case 0x2E:
    return 'c';
  case 0x20:
    return 'd';
  case 0x12:
    return 'e';
  case 0x21:
    return 'f';
  case 0x22:
    return 'g';
  case 0x23:
    return 'h';
  case 0x17:
    return 'i';
  case 0x24:
    return 'j';
  case 0x25:
    return 'k';
  case 0x26:
    return 'l';
  case 0x32:
    return 'm';
  case 0x31:
    return 'n';
  case 0x18:
    return 'o';
  case 0x19:
    return 'p';
  case 0x10:
    return 'q';
  case 0x13:
    return 'r';
  case 0x1F:
    return 's';
  case 0x14:
    return 't';
  case 0x16:
    return 'u';
  case 0x2F:
    return 'v';
  case 0x11:
    return 'w';
  case 0x2D:
    return 'x';
  case 0x15:
    return 'y';
  case 0x2C:
    return 'z';
  case 0x39:
    return ' ';
  case 0x02:
    return '1';
  case 0x03:
    return '2';
  case 0x04:
    return '3';
  case 0x05:
    return '4';
  case 0x06:
    return '5';
  case 0x07:
    return '6';
  case 0x08:
    return '7';
  case 0x09:
    return '8';
  case 0x0A:
    return '9';
  case 0x0B:
    return '0';
  default:
    return 0; // Handle unrecognized keycodes
  }
}

unsigned int get_len(unsigned char *ch) {
  unsigned int l = 0;

  while (*ch != '\0') {
    ch++;
    l++;
  }

  return l;
}

void backspace(unsigned char *buffer) {
  unsigned int len = get_len(buffer);
  if (len > 0) {
    buffer[len - 1] = '\0';
  }
}

void kernel_main(void) {
  terminal_initialize();

  unsigned char keyboard_buffer[256];
  unsigned int i = 0;

  for (i = 0; i < 256; i++) {
    keyboard_buffer[i] = '\0';
  }

  i = 0;
  while (1) {
    unsigned char scan_code = 0;

    while (scan_code == 0) {
      scan_code = get_key_press();
    }

    unsigned char ascii_char = keycode_to_ascii(scan_code);

    if (scan_code == 0x0E) {

      if (i > 0) {

        backspace(keyboard_buffer);
        i--;

        // TODO Backspace implement
      }
    }

    else if (ascii_char != 0) {
      keyboard_buffer[i] = ascii_char;
      i++;
      putchar(ascii_char);
    }

    while (get_key_press() == scan_code) {
    }
  }
}
