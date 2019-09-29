#include "..\lib\trigger_lib.cpp"

TeensyTrigger MyTrig;

void setup() {
  setup_serial();
  MyTrig.setup_io_pins();

  MyTrig.currentCommand = DO_NOTHING;
  trigOutChMask = 0b00000000;
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

void loop() {
  while(true){ // loop is slower than while(true)
    // read a command if one was send
    if (Serial.available() > 1) {
      MyTrig.currentCommand = serial_read_16bit();      // read the incoming byte:
    }

    // here starts our state machine
    switch (MyTrig.currentCommand) {
      // -----------------------------------------------------------------------
      case DO_NOTHING:
        break;
      // -----------------------------------------------------------------------
      case SET_TRIGGER_CH:
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
        serial_write_16bit(static_cast<uint16_t>(trigOutChMask)); // send the "ok, we are done" command
        serial_write_16bit(static_cast<uint16_t>(triggerOut)); // send the "ok, we are done" command
        serial_write_16bit(DONE); // send the "ok, we are done" command
        MyTrig.currentCommand = DO_NOTHING; // no need to send extra command?
        break;

      // -----------------------------------------------------------------------
      case DO_TRIGGER:
        doTrigger = true;
        // set static LEDS
        digitalWriteFast(DAQ_LED_PIN, HIGH);
        LED_PORT = trigOutChMask; // enable LEDS based on current trigger mode

        while(doTrigger)
        {
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
            if (Serial.available() >= 2)
            {
              MyTrig.currentCommand = serial_read_16bit_no_wait();
              if (MyTrig.currentCommand == STOP_TRIGGER)
              {
                doTrigger = false;
              }
            }
          }
        }
        LED_PORT = 0b00000000; // disable LEDs
        digitalWriteFast(DAQ_LED_PIN, LOW);
        MyTrig.currentCommand = DO_NOTHING; // exit state machine
        break;

      // -----------------------------------------------------------------------
      case ENABLE_SCOPE_MODE:
        slowMode = serial_read_16bit(); // delay in ms or us
        triggerPeriod = serial_read_16bit();
        nTrigger = serial_read_16bit(); // trigger how many times?
        lastCommandCheck = 0;
        triggerCounter = 0; // reset trigger counter
        doTrigger = true;
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
          TRIG_OUT_PORT = trigOutChMask; // enable triggers as prev. defined
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
              MyTrig.currentCommand = serial_read_16bit_no_wait();
              if (MyTrig.currentCommand == DISABLE_SCOPE)
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
        MyTrig.currentCommand = DO_NOTHING; // exit state machine
        break;

      case CHECK_CONNECTION:
        serial_write_16bit(DONE); // send the "ok, we are done" command
        MyTrig.currentCommand = DO_NOTHING; // exit state machine
        break;

      // -----------------------------------------------------------------------
      default:
        // statements
        MyTrig.currentCommand = DO_NOTHING; // exit state machine
        break;
    }
  }
}
