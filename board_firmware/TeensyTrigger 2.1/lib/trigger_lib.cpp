#include "trigger_lib.h"

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
    delay(50);
  }
  delay(200);
  LED_PORT = 0b00000000; // enable LEDS based on current trigger mode
  delay(200);
  LED_PORT = 0b11111111; // enable LEDS based on current trigger mode
  delay(200);
  LED_PORT = 0b00000000; // enable LEDS based on current trigger mode
}

// check for new command -------------------------------------------------------
uint_fast8_t TeensyTrigger::check_for_serial_command(){
  // read a command if one was send
  if (Serial.available() >= 2) {
    this->currentCommand = serial_read_16bit_no_wait(); // read the incoming byte:
    return 1;
  }
  else
    return 0;
}

// check for new command -------------------------------------------------------
void TeensyTrigger::set_trigger_channel(){
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

// check for new command -------------------------------------------------------
void TeensyTrigger::do_trigger(){
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
void TeensyTrigger::execute_serial_command(){
  // here starts our state machine
  switch (currentCommand) {
    // -----------------------------------------------------------------------
    case DO_NOTHING:
      break;
    // -----------------------------------------------------------------------
    case SET_TRIGGER_CH:
      this->set_trigger_channel();
      break;

    // -----------------------------------------------------------------------
    case DO_TRIGGER:
      this->do_trigger();
      break;

    // -----------------------------------------------------------------------
    case ENABLE_SCOPE_MODE:
      slowMode = serial_read_16bit(); // delay in ms or us
      triggerPeriod = serial_read_16bit();
      nTrigger = serial_read_16bit(); // trigger how many times?
      lastCommandCheck = 0;
      triggerCounter = 0; // reset trigger counter
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
            if (this->currentCommand == DISABLE_SCOPE)
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
      break;

    case CHECK_CONNECTION:
      serial_write_16bit(DONE); // send the "ok, we are done" command
      this->currentCommand = DO_NOTHING; // exit state machine
      break;

    // -----------------------------------------------------------------------
    default:
      // statements
      this->currentCommand = DO_NOTHING; // exit state machine
      break;
  }
}
