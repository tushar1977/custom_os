#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include "stdio.h"
#include <stdint.h>
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline const char *vga_to_ansi_color(enum vga_color color) {
  // ANSI color codes 0-15 correspond to VGA colors 0-15
  static const char *ansi_colors[] = {"30", "34", "32", "36", "31", "35",
                                      "33", "37", "90", "94", "92", "96",
                                      "91", "95", "93", "97"};
  return ansi_colors[color];
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

static inline void set_color(enum vga_color fg, enum vga_color bg) {
  printf("\033[%s;%sm", vga_to_ansi_color(fg), vga_to_ansi_color(bg));
}

// Function to reset colors (optional)
static inline void reset_color() {
  printf("\033[0m"); // Reset to default terminal colors
}

#endif
