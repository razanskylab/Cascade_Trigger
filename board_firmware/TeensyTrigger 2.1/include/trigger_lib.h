#ifndef TRIGGER_LIB
#define TRIGGER_LIB

#include <Arduino.h> // always required when using platformio
#include "wait.h" // always required when using platformio

// port B (GPIOB_PDOR/GPIOB_PDIR) -> used for trigger input
const uint8_t TRIG_IN_PINS[] = {16,17};
// port C (GPIOC_PDOR/GPIOC_PDIR) -> used for trigger output
const uint8_t TRIG_OUT_PINS[] = {15,22,23,9,10,13,11,12};
// port D (GPIOD_PDOR/GPIOD_PDIR) -> used for LED output
const uint8_t LED_OUT_PINS[] = {2,14,7,8,6,20,21,5};

// All below might not be needed or has not been tested...


#define ALL_TRIGGER_HIGH GPIOC_PDOR = 0b11111111; //
#define ALL_TRIGGER_LOW GPIOC_PDOR = 0b00000000; //
#define ALL_LED_HIGH GPIOD_PDOR = 0b11111111; //
#define ALL_LED_LOW GPIOD_PDOR = 0b00000000; //
#define ENABLE_1234 GPIOC_PDOR =  (GPIOC_PDOR & 0b00100111 ) | 0b11011000
  // enables trigger outputs 1 (bit 8), 2 (bit 7), 3 (bit 5) and 4 (bit 4)
  // should not affect the other bits (needs to be tested!)
#define DISABLE_1234 GPIOC_PDOR = (GPIOC_PDOR & 0b00100111 ) | 0b00000000 //

#define TRIG_IN_1 ((GPIOB_PDIR >> 0) & 1U) // trig in 1 = bit 1 = no shift
#define TRIG_IN_2 ((GPIOB_PDIR >> 1) & 1U) // trig in 2 = bit 2 = one shift

// PORTS and PIN fun %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define TRIG_IN_PORT GPIOB_PDIR

#define TRIG_IN1_HIGH (GPIOB_PDIR & (1UL << 0))
#define TRIG_IN1_LOW  (GPIOB_PDIR ^ (1UL << 0))
#define TRIG_IN1      (GPIOB_PDIR ^ (1UL << 0))

#define TRIG_IN2_HIGH GPIOB_PDIR & (1UL << 1)
#define TRIG_IN2_LOW GPIOB_PDIR ^ (1UL << 1)

#define TRIG_IN3_HIGH GPIOB_PDIR & (1UL << 3)
#define TRIG_IN3_LOW GPIOB_PDIR ^ (1UL << 3)

#define TRIG_IN4_HIGH GPIOB_PDIR & (1UL << 2)
#define TRIG_IN4_LOW GPIOB_PDIR ^ (1UL << 2)

#define TRIG_OUT_PORT GPIOC_PDOR
#define LED_PORT GPIOD_PDOR


// define commands %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define DO_NOTHING 00
#define SET_TRIGGER_CH 11
#define DO_TRIGGER 22
#define STOP_TRIGGER 23
#define CHECK_CONNECTION 88
#define ENABLE_SCOPE_MODE 66
#define DISABLE_SCOPE 67
#define DONE 99

// define trigger port bits %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define DAQ_TRIG 8
#define US_TRIG 7
#define ONDA_TRIG 5
#define EDGE_TRIG 4
#define DAQ_LED_PIN 3
#define ALL_TRIG 0
  // screwed up wiring there, bit 8 of the LED port isn't connected right...

uint32_t lastCommandCheck;
const uint16_t COMMAND_CHECK_INTERVALL = 100; // [ms] wait this long before checking for serial

uint8_t triggerOut = US_TRIG; //
  // bit 8 = DAQ_TRIG = TRIG 1
  // bit 7 = US_TRIG = TRIG 2
  // bit 5 = ONDA_TRIG = TRIG 3
  // bit 4 = EDGE_TRIG = TRIG 4
bool doTrigger = true; //
uint8_t lastTrigState = 0; //
uint8_t trigOutChMask = 0b00000000;

uint16_t slowMode; // delay in ms or us
uint16_t triggerPeriod;
uint16_t nTrigger; // trigger how many times?
uint32_t triggerCounter;
uint32_t lastSamplingTime; // used during stage calibration

// General Function Declarations
void setup_serial();

// Vital Class Definition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class TeensyTrigger {
  public:
    TeensyTrigger();
    ~TeensyTrigger();

    void setup_io_pins();
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // variables for analog & digital temperature measurements
    const uint16_t THERMISTORNOMINAL = 10000;
    uint16_t currentCommand = DO_NOTHING; // for incoming serial data

  private:

};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// function declarations
void setup_io_pins();
uint16_t serial_read_16bit();
uint16_t serial_read_16bit_no_wait();
void serial_write_16bit(uint16_t writeData);
void serial_wait_next_command();
void serial_write_32bit(uint32_t writeData);

#endif
