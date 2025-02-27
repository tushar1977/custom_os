#include "../include/keyboard.h"
#include "../include/idt.h"
#include "../include/memory.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/tty.h"
#include "../include/vfs.h"
#include "../include/vga.h"
#include "stdint.h"
#include <stdbool.h>

bool capsOn;
bool capsLock;

char text[100] = {0};

const uint32_t UNKNOWN = 0xFFFFFFFF;
const uint32_t ESC = 0xFFFFFFFF - 1;
const uint32_t CTRL = 0xFFFFFFFF - 2;
const uint32_t LSHFT = 0xFFFFFFFF - 3;
const uint32_t RSHFT = 0xFFFFFFFF - 4;
const uint32_t ALT = 0xFFFFFFFF - 5;
const uint32_t F1 = 0xFFFFFFFF - 6;
const uint32_t F2 = 0xFFFFFFFF - 7;
const uint32_t F3 = 0xFFFFFFFF - 8;
const uint32_t F4 = 0xFFFFFFFF - 9;
const uint32_t F5 = 0xFFFFFFFF - 10;
const uint32_t F6 = 0xFFFFFFFF - 11;
const uint32_t F7 = 0xFFFFFFFF - 12;
const uint32_t F8 = 0xFFFFFFFF - 13;
const uint32_t F9 = 0xFFFFFFFF - 14;
const uint32_t F10 = 0xFFFFFFFF - 15;
const uint32_t F11 = 0xFFFFFFFF - 16;
const uint32_t F12 = 0xFFFFFFFF - 17;
const uint32_t SCRLCK = 0xFFFFFFFF - 18;
const uint32_t HOME = 0xFFFFFFFF - 19;
const uint32_t UP = 0xFFFFFFFF - 20;
const uint32_t LEFT = 0xFFFFFFFF - 21;
const uint32_t RIGHT = 0xFFFFFFFF - 22;
const uint32_t DOWN = 0xFFFFFFFF - 23;
const uint32_t PGUP = 0xFFFFFFFF - 24;
const uint32_t PGDOWN = 0xFFFFFFFF - 25;
const uint32_t END = 0xFFFFFFFF - 26;
const uint32_t INS = 0xFFFFFFFF - 27;
const uint32_t DEL = 0xFFFFFFFF - 28;
const uint32_t CAPS = 0xFFFFFFFF - 29;
const uint32_t NONE = 0xFFFFFFFF - 30;
const uint32_t ALTGR = 0xFFFFFFFF - 31;
const uint32_t NUMLCK = 0xFFFFFFFF - 32;

char *slice(const char *str, size_t start, size_t end) {
  size_t len = end - start;
  char *result = (char *)malloc(len + 1);
  if (!result) {
    return NULL;
  }
  memcpy(result, str + start, len);
  result[len] = '\0';
  return result;
}
const uint32_t lowercase[128] = {
    UNKNOWN, ESC,     '1',     '2',     '3',     '4',     '5',     '6',
    '7',     '8',     '9',     '0',     '-',     '=',     '\b',    '\t',
    'q',     'w',     'e',     'r',     't',     'y',     'u',     'i',
    'o',     'p',     '[',     ']',     '\n',    CTRL,    'a',     's',
    'd',     'f',     'g',     'h',     'j',     'k',     'l',     ';',
    '\'',    '`',     LSHFT,   '\\',    'z',     'x',     'c',     'v',
    'b',     'n',     'm',     ',',     '.',     '/',     RSHFT,   '*',
    ALT,     ' ',     CAPS,    F1,      F2,      F3,      F4,      F5,
    F6,      F7,      F8,      F9,      F10,     NUMLCK,  SCRLCK,  HOME,
    UP,      PGUP,    '-',     LEFT,    UNKNOWN, RIGHT,   '+',     END,
    DOWN,    PGDOWN,  INS,     DEL,     UNKNOWN, UNKNOWN, UNKNOWN, F11,
    F12,     UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN};

const uint32_t uppercase[128] = {
    UNKNOWN, ESC,     '!',     '@',     '#',     '$',     '%',     '^',
    '&',     '*',     '(',     ')',     '_',     '+',     '\b',    '\t',
    'Q',     'W',     'E',     'R',     'T',     'Y',     'U',     'I',
    'O',     'P',     '{',     '}',     '\n',    CTRL,    'A',     'S',
    'D',     'F',     'G',     'H',     'J',     'K',     'L',     ':',
    '"',     '~',     LSHFT,   '|',     'Z',     'X',     'C',     'V',
    'B',     'N',     'M',     '<',     '>',     '?',     RSHFT,   '*',
    ALT,     ' ',     CAPS,    F1,      F2,      F3,      F4,      F5,
    F6,      F7,      F8,      F9,      F10,     NUMLCK,  SCRLCK,  HOME,
    UP,      PGUP,    '-',     LEFT,    UNKNOWN, RIGHT,   '+',     END,
    DOWN,    PGDOWN,  INS,     DEL,     UNKNOWN, UNKNOWN, UNKNOWN, F11,
    F12,     UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
    UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN};

void clear() {
  uint8_t p = 0;
  // Loop through the text buffer and set each character to '\0' (null
  // terminator)
  while (text[p] != '\0') {
    text[p] = '\0';
    p++;
  }
}

void keyboardHandler(struct InterruptRegisters *regs) {
  char scanCode = inPortB(0x60) & 0x7F;
  char press = inPortB(0x60) & 0x80;
  // printf("%d", scanCode);

  switch (scanCode) {
  case 1:
  case 29:
  case 56:
  case 59:
  case 60:
  case 61:
  case 62:
  case 63:
  case 64:
  case 65:
  case 66:
  case 67:
  case 68:
  case 87:
  case 88:
    break;
  case 42:
    if (press == 0) {
      capsOn = true;
    } else {
      capsOn = false;
    }
    break;
  case 28:
    if (press == 0) {
      newline();

      if (strcmp("clear", text) == 0) {
        Reset();
      }

      else if (strcmp("ls", text) == 0) {
        printf("__FILES__\n");
        display_files();
        printf("\n");
      }

      else if (memcmp("echo ", text, strlen("echo ")) == 0) {
        char *command = text + strlen("echo ");
        char *first_arg = memchr2(command, '\'');

        if (first_arg) {
          char *second_qot = memchr2(first_arg + 1, '\'');
          if (second_qot) {
            *second_qot = '\0';
            char *data = first_arg + 1;
            char *filename = second_qot + 2;

            while (*filename == ' ')
              filename++;

            if (*filename != '\0') {
              insert_data(filename, data);
              printf("Inserted Data to %s\n", filename);
            } else {
              printf("Error! No filename provided\n");
              free(second_qot);
            }
          } else {
            printf("Error! Missing closing quote\n");

            free(second_qot);
          }
        } else {
          printf("Error! Invalid echo format\n");
        }

        free(first_arg);
      } else if (memcmp("cat", text, strlen("cat")) == 0) {
        char *filename = NULL;
        filename = slice(text, strlen("cat") + 1, strlen(text));
        if (filename) {
          display_file_content(filename);
          free(filename);
        } else {
          printf("Error: Could not process filename.\n");
        }
      }

      else if (memcmp("mfile", text, strlen("mfile")) == 0) {
        char *filename = slice(text, strlen("mfile") + 1, strlen(text));
        create_file(filename, "");
        printf("Done created %s\n", filename);
        free(filename);
      }

      else if (scanCode != 28) {
        printf("Unknown command: %s\n", text);
      }

      clear();

      printf("tusos--> ");
    }
    break;
  case 58:
    if (!capsLock && press == 0) {
      capsLock = true;
    } else if (capsLock && press == 0) {
      capsLock = false;
    }
    break;
  default:
    if (press == 0) {
      if (scanCode >= 0 && scanCode < 128) {
        if (capsOn) {
          printf("%c", uppercase[scanCode]);
        } else if (capsLock) {
          printf("%c", (scanCode >= 'a' && scanCode <= 'z')
                           ? uppercase[scanCode]
                           : lowercase[scanCode]);
        } else {
          printf("%c", lowercase[scanCode]);
        }

        int i = 0;
        while (text[i] != '\0' && i < sizeof(text) - 1) {
          i++;
        }
        if (i < sizeof(text) - 1) {
          text[i] =
              (capsOn || capsLock) ? uppercase[scanCode] : lowercase[scanCode];
          text[i + 1] = '\0';
        }
      }
    }
  }
}

void initKeyboard() {
  capsOn = false;
  capsLock = false;
  irq_install_handler(1, &keyboardHandler);
}
