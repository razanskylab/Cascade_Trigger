#ifndef TEENSY_LIB
#define TEENSY_LIB

#include <Arduino.h> // always required when using platformio

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// function declarations
void setup_io_pins();
uint16_t serial_read_16bit();
uint16_t serial_read_16bit_no_wait();
void serial_write_16bit(uint16_t writeData);
void serial_wait_next_command();


// wait a few clock cycles
#define NOP __asm__ __volatile__ ("nop\n\t") // ~6 ns = one clock cycle
#define WAIT_10_NS NOP; NOP;
#define WAIT_20_NS NOP; NOP; NOP; NOP;
#define WAIT_40_NS WAIT_20_NS; WAIT_20_NS;
#define WAIT_60_NS WAIT_20_NS; WAIT_20_NS; WAIT_20_NS;
#define WAIT_80_NS WAIT_40_NS; WAIT_40_NS;
#define WAIT_100_NS WAIT_80_NS; WAIT_20_NS;

// define commands %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define DO_NOTHING 00
#define DO_TRIGGER 11
#define CHECK_CONNECTION 98
#define DONE 99

uint32_t lastCommandCheck;
uint16_t currentCommand = DO_TRIGGER; // for incoming serial data
const uint16_t COMMAND_CHECK_INTERVALL = 100; // [ms] wait this long before checking for serial



// PORTS and PIN fun %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// port B (GPIOB_PDOR/GPIOB_PDIR) -> used for trigger input
const uint8_t PORT_B_PINS[] = {16,17,18,19};
// trigger input Port to teensy Port mapping -> PORT_B_PINS
// trigger in | teensy trigger in PIN | teensy port bit
// 1 | 16 | 0
// 2 | 17 | 1
// 3 | 18 | 3
// 4 | 19 | 2
#define TRIG_IN1_HIGH GPIOB_PDIR & (1UL << 0)
#define TRIG_IN1_LOW GPIOB_PDIR ^ (1UL << 0)

#define TRIG_IN2_HIGH GPIOB_PDIR & (1UL << 1)
#define TRIG_IN2_LOW GPIOB_PDIR ^ (1UL << 1)

#define TRIG_IN3_HIGH GPIOB_PDIR & (1UL << 3)
#define TRIG_IN3_LOW GPIOB_PDIR ^ (1UL << 3)

#define TRIG_IN4_HIGH GPIOB_PDIR & (1UL << 2)
#define TRIG_IN4_LOW GPIOB_PDIR ^ (1UL << 2)

// port C (GPIOC_PDOR/GPIOC_PDIR) -> used for trigger output
const uint8_t PORT_C_PINS[] = {15,22,23,9,10,13,11,12};
// trigger output Port to teensy Port mapping -> PORT_C_PINS
// trigger out | teensy trigger out PIN | teensy port bit
// 1 | 12 | 8
// 2 | 11 | 7
// 3 | 10 | 5
// 4 | 9  | 4
// 5 | 23 | 3
// 6 | 22 | 2
// 7 | 15 | 1
// 8 | 13 | 6


// port D (GPIOD_PDOR/GPIOD_PDIR) -> used for LED output
const uint8_t PORT_D_PINS[] = {2,14,7,8,6,20,21,5};
// triggerPort to LED mapping -> PORT_D_PINS
// led out | teensy LED PIN | teensy port bit
// 1 | 5  | 8
// 2 | 21 | 7
// 3 | 6  | 5
// 4 | 8  | 4
// 5 | 7  | 3
// 6 | 14 | 2
// 7 | 2  | 1
// 8 | 20 | 6


// this is how we set, clear and toggle trigger ports
// triggerPort |= (1UL << 0); // set 0st bit = first port
// triggerPort &= ~(1UL << 2); // clear 2nd bit = third port
// triggerPort ^= (1UL << 4); // toggle 4th bit = 5th port





#endif
