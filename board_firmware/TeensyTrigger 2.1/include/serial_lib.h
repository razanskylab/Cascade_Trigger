#ifndef SERIAL_LIB
#define SERIAL_LIB

#include <Arduino.h> // always required when using platformio

uint16_t serial_read_16bit();
uint16_t serial_read_16bit_no_wait();
void serial_write_16bit(uint16_t writeData);
void serial_wait_next_command();
void serial_write_32bit(uint16_t writeData);

#endif
