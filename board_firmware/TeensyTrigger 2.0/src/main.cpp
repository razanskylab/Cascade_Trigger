#include "..\lib\teensy_lib.cpp"

uint8_t triggerOut = US_TRIG; //
  // bit 8 = DAQ_TRIG = TRIG 1
  // bit 7 = US_TRIG = TRIG 2
  // bit 5 = ONDA_TRIG = TRIG 3
  // bit 4 = EDGE_TRIG = TRIG 4
bool doTrigger = true; //
uint8_t lastTrigState = 0; //
uint8_t trigOutChMask = 0b00000000;

void setup() {
  setup_io_pins();
  Serial.begin(9600);
  currentCommand = SET_TRIGGER_CH;
  trigOutChMask = 0b00000000;
  for (int iLed = 0; iLed<8; iLed++)
  {
    digitalWriteFast(PORT_D_PINS[iLed],HIGH);
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
  while(true){
    // read a command if one was send
    if (Serial.available() > 1) {
      currentCommand = serial_read_16bit();      // read the incoming byte:
    }

    // here starts our state machine
    switch (currentCommand) {
      // -----------------------------------------------------------------------
      case SET_TRIGGER_CH:
        // FIXME uncomment this!
        triggerOut = static_cast<uint8_t>(serial_read_16bit());
        trigOutChMask = 0b00000000;
        trigOutChMask |= (1UL << DAQ_TRIG-1); // always tigger DAQ
        trigOutChMask |= (1UL << triggerOut-1); // trigger US or Onda or Dye
        serial_write_16bit(DONE); // send the "ok, we are done" command
        currentCommand = DO_NOTHING; // no need to send extra command?
        break;

      case DO_TRIGGER:
        doTrigger = true;
        // set static LEDS
        digitalWriteFast(DAQ_LED_PIN, HIGH);
        LED_PORT = trigOutChMask; // enable LEDS based on current trigger mode

        while(doTrigger){
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
              currentCommand = serial_read_16bit_no_wait();
              if (currentCommand == STOP_TRIGGER)
              {
                doTrigger = false;
              }
            }
          }
        }
        LED_PORT = 0b00000000; // enable LEDS based on current trigger mode
        digitalWriteFast(DAQ_LED_PIN, LOW);
        currentCommand = DO_NOTHING; // exit state machine
        break;

      case CHECK_CONNECTION:
        serial_write_16bit(DONE); // send the "ok, we are done" command
        currentCommand = DO_NOTHING; // exit state machine
        break;

      // -----------------------------------------------------------------------
      default:
        // statements
        currentCommand = DO_NOTHING; // exit state machine
        break;
    }
  }
}



// this is how we set, clear and toggle trigger ports
// triggerPort |= (1UL << 0); // set 0st bit = first port
// triggerPort &= ~(1UL << 2); // clear 2nd bit = third port
// triggerPort ^= (1UL << 4); // toggle 4th bit = 5th port

// while (true){
//   // instead of storring our trigger state, we use a super simple
//   // state machine
//   while(TRIG_IN1_LOW){
//   }
//   GPIOC_PDOR = 0b11111111; //
//   GPIOD_PDOR = 0b11111111; //
//   WAIT_100_NS;
//
//   GPIOC_PDOR = 0b00000000; //
//   GPIOD_PDOR = 0b00000000; //
//
//   while(TRIG_IN1_HIGH){
//   }
//
//   GPIOC_PDOR = 0b11111111; //
//   GPIOD_PDOR = 0b11111111; //
//   WAIT_100_NS;
//
//   GPIOC_PDOR = 0b00000000; //
//   GPIOD_PDOR = 0b00000000; //
// }
