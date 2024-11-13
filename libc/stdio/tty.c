#include "../include/tty.h"
#include "../include/string.h"
#include "../include/util.h"
#include "../include/vga.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
  update_cursor(0, 0);
}
void scrollUp() {
  for (size_t y = 1; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      terminal_buffer[(y - 1) * VGA_WIDTH + x] =
          terminal_buffer[y * VGA_WIDTH + x];
    }
  }

  for (size_t x = 0; x < VGA_WIDTH; x++) {
    terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
        vga_entry(' ', terminal_color);
  }
}

void Reset() {
  terminal_row = 0;
  terminal_column = 0;

  for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
    for (uint16_t x = 0; x < VGA_WIDTH; x++) {

      terminal_buffer[y * VGA_WIDTH + x] = ' ' | (terminal_color << 8);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void update_cursor(int x, int y) {
  uint16_t pos = y * VGA_WIDTH + x;

  outPortB(0x3D4, 0x0F);
  outPortB(0x3D5, (uint8_t)(pos & 0xFF));
  outPortB(0x3D4, 0x0E);
  outPortB(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}
void newline() {
  terminal_column = 0;
  if (++terminal_row == VGA_HEIGHT) {
    scrollUp();
    terminal_row = VGA_HEIGHT - 1;
  }

  update_cursor(terminal_column, terminal_row);
}

void backspace() {
  if (terminal_column == 9) {
    size_t index = terminal_row * VGA_WIDTH + (++terminal_column);
    terminal_buffer[index] = ' ' | (terminal_color << 8);
  }
  if (terminal_column == 0 && terminal_row != 0) {
    terminal_row--;
    terminal_column = VGA_WIDTH;
  }
  size_t index = terminal_row * VGA_WIDTH + (--terminal_column);
  terminal_buffer[index] = ' ' | (terminal_color << 8);
  update_cursor(terminal_column, terminal_row);
}
void terminal_putchar(char c) {

  if (c == '\n') {
    newline();
    return;
  }
  if (c == '\b') {
    backspace();
    return;
  }

  update_cursor(terminal_column, terminal_row);
  unsigned char uc = c;
  terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {

    newline();
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {

  terminal_write(data, strlen(data));
}
