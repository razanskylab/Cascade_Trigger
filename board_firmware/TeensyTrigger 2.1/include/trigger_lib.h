#ifndef TRIGGER_LIB
#define TRIGGER_LIB

#include <Arduino.h> // always required when using platformio
#include "..\lib\serial_lib.cpp"
#include "wait.h" // always required when using platformio

// port B (GPIOB_PDOR/GPIOB_PDIR) -> used for trigger input
const uint8_t TRIG_IN_PINS[] = {16,17};
// port C (GPIOC_PDOR/GPIOC_PDIR) -> used for trigger output
const uint8_t TRIG_OUT_PINS[] = {15,22,23,9,10,13,11,12};
// port D (GPIOD_PDOR/GPIOD_PDIR) -> used for LED output
const uint8_t LED_OUT_PINS[] = {2,14,7,8,6,20,21,5};


// Define serial communication commands (shared with matlab)
const uint_fast16_t DONE = 99;
const uint_fast16_t READY_FOR_COMMAND = 98;
const uint_fast16_t DO_NOTHING = 0;
const uint_fast16_t SET_TRIGGER_CH = 11;
const uint_fast16_t EXT_TRIGGER = 22;
const uint_fast16_t STOP_TRIGGER = 23;
const uint_fast16_t CHECK_CONNECTION = 88;
const uint_fast16_t ENABLE_INT_TRIGGER = 66;
const uint_fast16_t DISABLE_INT_TRIGGER = 67;


constexpr uint_fast8_t AOD_PIN = 3;
constexpr uint_fast8_t CAM_PIN = 2;


// All below might not be needed or has not been tested... %%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define ALL_TRIGGER_HIGH GPIOC_PDOR = 0b11111111; //
#define ALL_TRIGGER_LOW GPIOC_PDOR = 0b00000000; //
#define ALL_LED_HIGH GPIOD_PDOR = 0b11111111; //
#define ALL_LED_LOW GPIOD_PDOR = 0b00000000; //
#define ENABLE_1234 GPIOC_PDOR =  (GPIOC_PDOR & 0b00100111 ) | 0b11011000
  // enables trigger outputs 1 (bit 8), 2 (bit 7), 3 (bit 5) and 4 (bit 4)
  // should not affect the other bits (needs to be tested!)
#define DISABLE_1234 GPIOC_PDOR = (GPIOC_PDOR & 0b00100111 ) | 0b00000000

#define TRIG_IN_1 ((GPIOB_PDIR >> 0) & 1U) // trig in 1 = bit 1 = no shift
#define TRIG_IN_2 ((GPIOB_PDIR >> 1) & 1U) // trig in 2 = bit 2 = one shift

// PORTS and PIN fun %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define TRIG_IN_PORT GPIOB_PDIR
#define TRIG_OUT_PORT GPIOC_PDOR
#define LED_PORT GPIOD_PDOR

#define TRIG_IN1_HIGH (GPIOB_PDIR & (1UL << 0))
#define TRIG_IN1_LOW  (GPIOB_PDIR ^ (1UL << 0))
#define TRIG_IN1      (GPIOB_PDIR ^ (1UL << 0))

#define TRIG_IN2_HIGH GPIOB_PDIR & (1UL << 1)
#define TRIG_IN2_LOW GPIOB_PDIR ^ (1UL << 1)



// define trigger port bits %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define DAQ_TRIG 8
#define US_TRIG 7
#define ONDA_TRIG 5
#define EDGE_TRIG 4
#define ALL_TRIG 0
#define DAQ_LED_PIN 3
  // screwed up wiring there, bit 8 of the LED port isn't connected right...

uint32_t lastSamplingTime; // used during stage calibration

// Vital Class Definition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class TeensyTrigger {
  public:
    inline TeensyTrigger();
    inline ~TeensyTrigger();

    inline void setup_io_pins();
    inline void show_led_welcome();
    FASTRUN uint_fast8_t check_for_serial_command();
    FASTRUN void set_trigger_channel();
    FASTRUN void external_trigger();
    FASTRUN void stand_alone_trigger();
    FASTRUN void chen_stand_alone_trigger();
    FASTRUN void enable_trigger_output(uint_fast8_t triggerBit);
    FASTRUN void disable_trigger_output(uint_fast8_t triggerBit);
    // FASTRUN void execute_serial_command();
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // variables for analog & digital temperature measurements
    const uint_fast16_t COMMAND_CHECK_INTERVALL = 200; // [ms] wait this long before checking for serial

    uint8_t trigOutChMask = 0b00000000;
    uint_fast16_t currentCommand = DO_NOTHING; // for incoming serial data
    uint8_t triggerOut = US_TRIG;
      // bit 8 = DAQ_TRIG = TRIG 1
      // bit 7 = US_TRIG = TRIG 2
      // bit 5 = ONDA_TRIG = TRIG 3
      // bit 4 = EDGE_TRIG = TRIG 4
      // sets trigger output channel
      // FIXME -> needs to be replaced with proper trigger mask etc..
    uint_fast8_t lastTrigState = 0;
      // keeps last external trigger state to detect stage changes
    uint_fast32_t lastCommandCheck = 0;

  private:

};

#endif
