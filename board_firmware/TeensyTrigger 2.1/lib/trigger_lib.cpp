#include "trigger_lib.h"

//NanoDelay %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// delay for a given number of nano seconds
// less sensitive to interrupts and DMA
// max delay is 4 seconds
// NOTE:  minimum pulse width is ~700 nsec, accuracy is ~ -0/+40 ns
// NOTE:  you can't trust this code:
//        compiler or library changes will change timing overhead
//        CPU speed will effect timing

// prepare before, so less delay later
static uint32_t nano_ticks;

// constexpr double CLOCK_RATE = 240.0E6; // MCU clock rate - measure it for best accuracy
constexpr double CLOCK_RATE = 240.0E6; // MCU clock rate - measure it for best accuracy
// constexpr unsigned NANO_OVERHEAD = 470;         // overhead - adjust as needed
constexpr unsigned NANO_OVERHEAD = 130;         // overhead - adjust as needed
// constexpr unsigned NANO_JITTER = 18;            // adjusts for jitter prevention - leave at 18
constexpr unsigned NANO_JITTER = 0;            // adjusts for jitter prevention - leave at 18

void setup_nano_delay(uint32_t nanos)
{
  // set up cycle counter
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

  // improve teensy 3.1 clock accuracy
  OSC0_CR = 0x2;

  if (nanos < NANO_OVERHEAD)   // we can't do less than this
     nanos = NANO_OVERHEAD;

  // how many cycles to wait
  nano_ticks = ((nanos - NANO_OVERHEAD) / (1.0E9 / CLOCK_RATE)) + .5;

  if (nano_ticks < NANO_JITTER)
     nano_ticks = NANO_JITTER;

} // Setup_Nano_Delay()

// Do the delay specified above.
// You may want to disable interrupts before and after
FASTRUN void wait_nano_delay(void)
{
  uint32_t start_time = ARM_DWT_CYCCNT;
  uint32_t loop_ticks = nano_ticks - NANO_JITTER;

  // loop until time is almost up
  while ((ARM_DWT_CYCCNT - start_time) < loop_ticks) {
     // could do other things here
  }

  if (NANO_JITTER) {   // compile time option
    register unsigned r;          // for debugging

    // delay for the remainder using single instructions
    switch (r = (nano_ticks - (ARM_DWT_CYCCNT - start_time))) {
      case 18: __asm__ volatile("nop" "\n\t");
      case 17: __asm__ volatile("nop" "\n\t");
      case 16: __asm__ volatile("nop" "\n\t");
      case 15: __asm__ volatile("nop" "\n\t");
      case 14: __asm__ volatile("nop" "\n\t");
      case 13: __asm__ volatile("nop" "\n\t");
      case 12: __asm__ volatile("nop" "\n\t");
      case 11: __asm__ volatile("nop" "\n\t");
      case 10: __asm__ volatile("nop" "\n\t");
      case 9: __asm__ volatile("nop" "\n\t");
      case 8: __asm__ volatile("nop" "\n\t");
      case 7: __asm__ volatile("nop" "\n\t");
      case 6: __asm__ volatile("nop" "\n\t");
      case 5: __asm__ volatile("nop" "\n\t");
      case 4: __asm__ volatile("nop" "\n\t");
      case 3: __asm__ volatile("nop" "\n\t");
      case 2: __asm__ volatile("nop" "\n\t");
      case 1: __asm__ volatile("nop" "\n\t");
      default:
           break;
    }  // switch()
  } // if
}  // Nano_Delay()

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TeensyTrigger::TeensyTrigger(){}
TeensyTrigger::~TeensyTrigger(){}

// setup IO PINs ---------------------------------------------------------------
void TeensyTrigger::setup_io_pins(){
  // set two trigger inputs as digital input pins
  pinMode(TRIG_IN_PINS[0],INPUT);
  pinMode(TRIG_IN_PINS[1],INPUT);
  // set 8 trigger outputs and corresponding LEDs as digital output pins
  for (int i=0; i<8; i++){
    pinMode(TRIG_OUT_PINS[i],OUTPUT);
    pinMode(LED_OUT_PINS[i],OUTPUT);
  }
  pinMode(DAQ_LED_PIN,OUTPUT); // NOTE note sure why we need this...
  this->show_led_welcome();
}

// make led blink --------------------------------------------------------------
void TeensyTrigger::show_led_welcome(){
  for (int iLed = 0; iLed<8; iLed++)
  {
    digitalWriteFast(LED_OUT_PINS[iLed],HIGH);
    delay(25);
  }
  delay(50);
  LED_PORT = 0b00000000; // enable LEDS based on current trigger mode
  delay(50);
  LED_PORT = 0b11111111; // enable LEDS based on current trigger mode
  delay(50);
  LED_PORT = 0b00000000; // enable LEDS based on current trigger mode
}

// check for new command -------------------------------------------------------
FASTRUN uint_fast8_t TeensyTrigger::check_for_serial_command(){
  // read a command if one was send
  if (Serial.available() >= 2) {
    this->currentCommand = serial_read_16bit_no_wait(); // read the incoming byte:
    return 1;
  }
  else
    return 0;
}

// check for new command -------------------------------------------------------
FASTRUN void TeensyTrigger::set_trigger_channel(){
  // FIXME uncomment this!
  triggerOut = static_cast<uint8_t>(serial_read_16bit());
  if (triggerOut == ALL_TRIG){
    trigOutChMask = 0b11111111;
  }
  else {
    trigOutChMask = 0b00000000;
    trigOutChMask |= (1UL << (DAQ_TRIG-1)); // always tigger DAQ
    trigOutChMask |= (1UL << (triggerOut-1)); // trigger US or Onda or Dye
  }
  // write back what we just did as a way of error checking
  serial_write_16bit(static_cast<uint16_t>(trigOutChMask));
  serial_write_16bit(static_cast<uint16_t>(triggerOut));
  serial_write_16bit(DONE); // send the "ok, we are done" command
  currentCommand = DO_NOTHING; // no need to send extra command?
}

// trigger cascade following external trigger ----------------------------------
FASTRUN void TeensyTrigger::external_trigger(){
  uint_fast32_t lastCommandCheck = 0;
  uint_fast8_t doTrigger = true;
  // set static LEDS
  digitalWriteFast(DAQ_LED_PIN, HIGH);
  LED_PORT = trigOutChMask; // enable LEDS based on current trigger mode

  while(doTrigger)
  {
    // triggers on rising and falling flank
    // FIXME -> change this or make it optional!!!
    if (TRIG_IN1 != lastTrigState){
      TRIG_OUT_PORT = trigOutChMask; // enable triggers as prev. defined
      lastTrigState = !lastTrigState;
      delayMicroseconds(1);
      TRIG_OUT_PORT = 0b00000000; // disable all trigger
    }

    // check if we got a new serial command to stop triggering
    // COMMAND_CHECK_INTERVALL is high, so we only check once in a while
    if((millis()-lastCommandCheck) >= COMMAND_CHECK_INTERVALL)
    {
      lastCommandCheck = millis();
      check_for_serial_command();
      if (currentCommand == STOP_TRIGGER)
        doTrigger = false;
    }
  }
  LED_PORT = 0b00000000; // disable LEDs
  digitalWriteFast(DAQ_LED_PIN, LOW);
  currentCommand = DO_NOTHING; // exit state machine
}

// check for new command -------------------------------------------------------
FASTRUN void TeensyTrigger::stand_alone_trigger(){
  uint16_t slowMode = serial_read_16bit(); // delay in ms or us
  uint16_t triggerPeriod = serial_read_16bit();
  uint_fast32_t nTrigger = static_cast<uint_fast32_t>(serial_read_16bit()); // trigger how many times?
  uint_fast32_t lastCommandCheck = 0;
  uint_fast32_t triggerCounter = 0; // reset trigger counter
  uint_fast8_t doTrigger = true;
  // lastSamplingTime = 0;

  while (doTrigger)
  {
    // wait for next trigger point, we do this at least once!
    if (slowMode){
      while((millis()-lastSamplingTime)<triggerPeriod){};
      lastSamplingTime = millis();
    }
    else{
      while((micros()-lastSamplingTime)<triggerPeriod){};
      lastSamplingTime = micros();
    }

    // acutal triggering happens here, but using trigger ports
    TRIG_OUT_PORT = this->trigOutChMask; // enable triggers as prev. defined
    delayMicroseconds(1);
    TRIG_OUT_PORT = 0b00000000; // disable all trigger
    triggerCounter++;

    // if nTrigger = 0 we trigger indefinately
    if (nTrigger && (triggerCounter >= nTrigger)){
      doTrigger = false;
    }

    // check if we got a new serial command to stop triggering
    // COMMAND_CHECK_INTERVALL is high, so we only check once in a while
    if((millis()-lastCommandCheck) >= COMMAND_CHECK_INTERVALL)
    {
      lastCommandCheck = millis();
      if (Serial.available() >= 2)
      {
        this->currentCommand = serial_read_16bit_no_wait();
        if (this->currentCommand == DISABLE_INT_TRIGGER)
        {
          doTrigger = false;
        }
      }
    }
  }
  serial_write_16bit(DONE); // send the "ok, we are done" command
  serial_write_32bit(triggerCounter);
  LED_PORT = 0b00000000; // disable LEDs
  digitalWriteFast(DAQ_LED_PIN, LOW);
  this->currentCommand = DO_NOTHING; // exit state machine
}

// custom trigger function for chen to trigger AOD and camera only -------------
FASTRUN void TeensyTrigger::chen_stand_alone_trigger(){
  uint_fast32_t lastCommandCheck = 0;
  uint_fast32_t triggerCounter = 0;
  uint_fast8_t doTrigger = true;

  uint_fast32_t nTrigger = serial_read_32bit(); // trigger how many times?
  uint_fast32_t triggerFreq = serial_read_32bit(); // trigger freq. in Hz
  uint_fast32_t postAcqDelay = serial_read_32bit();
  // uint16_t nTrigger = serial_read_16bit(); // trigger how many times?
  // uint16_t triggerFreq = serial_read_16bit(); // trigger freq. in Hz
  // uint16_t postAcqDelay = serial_read_16bit();

    // delay after acq. is done for camera to prepare for next frame
  uint_fast32_t triggerPeriod = 1/(triggerFreq*1E-9); // trigger period in ns
  // we wait 50% of the trigger period on, then of, so we need half the actual period
  triggerPeriod = triggerPeriod/2;
  setup_nano_delay(triggerPeriod);

  while (doTrigger){
    for (uint_fast8_t iTrig = 0; iTrig < nTrigger; iTrig++) {
      GPIOC_PDOR = 0b00000110;
      wait_nano_delay();
      GPIOC_PDOR = 0b00000010;
      wait_nano_delay();
    }
    GPIOC_PDOR = 0b00000000;
    triggerCounter++;
    delayMicroseconds(postAcqDelay);
    // check if we got a new serial command to stop triggering
    // COMMAND_CHECK_INTERVALL is high, so we only check once in a while
    if((millis()-lastCommandCheck) >= COMMAND_CHECK_INTERVALL)
    {
      lastCommandCheck = millis();
      if (Serial.available() >= 2)
      {
        this->currentCommand = serial_read_16bit_no_wait();
        if (this->currentCommand == DISABLE_INT_TRIGGER)
          doTrigger = false;
      }
    }
  } // while (doTrigger)
  serial_write_16bit(DONE); // send the "ok, we are done" command
  serial_write_32bit(triggerCounter);
  LED_PORT = 0b00000000; // disable LEDs
  digitalWriteFast(DAQ_LED_PIN, LOW);
  this->currentCommand = DO_NOTHING; // exit state machine
}


// enable_trigger_output -------------------------------------------------------
FASTRUN void TeensyTrigger::enable_trigger_output(uint_fast8_t triggerBit){
  TRIG_OUT_PORT |= (1UL << (triggerBit-1));
}
// disable_trigger_output -------------------------------------------------------
FASTRUN void TeensyTrigger::disable_trigger_output(uint_fast8_t triggerBit){
  TRIG_OUT_PORT &= ~(1UL << (triggerBit-1));
}


// check for new command -------------------------------------------------------
// void TeensyTrigger::execute_serial_command(){
//   // here starts our state machine
//   switch (this->currentCommand) {
//     // -----------------------------------------------------------------------
//     case DO_NOTHING:
//       break;
//
//     // -----------------------------------------------------------------------
//     case SET_TRIGGER_CH:
//       this->set_trigger_channel();
//       break;
//
//     // -----------------------------------------------------------------------
//     case EXT_TRIGGER:
//       this->external_trigger();
//       break;
//
//     // -----------------------------------------------------------------------
//     case ENABLE_INT_TRIGGER:
//       this->stand_alone_trigger();
//       break;
//
//     case CHECK_CONNECTION:
//       serial_write_16bit(DONE); // send the "ok, we are done" command
//       this->currentCommand = DO_NOTHING; // exit state machine
//       break;
//
//     // -----------------------------------------------------------------------
//     default:
//       // statements
//       this->currentCommand = DO_NOTHING; // exit state machine
//       break;
//   }
// }
