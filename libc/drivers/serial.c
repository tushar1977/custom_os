#include "../include/serial.h"
#include "../include/util.h"
#include <stdbool.h>
#include <stdio.h>
#define PORT 0x3E8
static int init_serial() {

  outPortB(PORT + 1, 0x00); // Disable all interrupts
  outPortB(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outPortB(PORT + 0, 0x01); // Set divisor to 3 (lo byte) 38400 baud
  outPortB(PORT + 1, 0x00); //                  (hi byte)
  outPortB(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outPortB(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outPortB(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outPortB(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outPortB(PORT + 0, 0xAE);

  outPortB(PORT + 4, 0x0F);
  return 0;
}

uint8_t serial_line_status() { return inPortB(PORT + 5); }

bool is_transmit_empty() { return inPortB(PORT + 5) & 0x20; }
void serial_write(uint8_t data) {
  while (!is_transmit_empty())
    ;
  outPortB(PORT, data);
}

bool serial_received() { return inPortB(PORT + 5) & 1; }

uint8_t serial_read() {
  while (!serial_received())
    ;
  return inPortB(PORT);
}

void test_serial() {
  if (init_serial() != 0) {
    serial_write('E');
    printf("error");
    return;
  }

  const char *test_msg = "Hello, Serial!";
  for (const char *p = test_msg; *p != '\0'; p++) {
    serial_write(*p);
  }

  printf("Data written to serial :- %s\n", test_msg);

  uint8_t line_status = serial_line_status();
  printf("Line Status: 0x%x\n", line_status);
  if (serial_received()) {
    uint8_t data = serial_read();
    serial_write(data);
    printf("Data received: %d (0x%x)\nSerial Test done!\n", data, data);
  }
}
