#include "..\lib\teensy_lib.cpp"

void setup() {
  setup_io_pins();
  Serial.begin(9600);

}


void loop() {
  while(true){
    // read a command if one was send
    if (Serial.available() > 1) {
      currentCommand = serial_read_16bit();      // read the incoming byte:
    }

    if (currentCommand == DO_NOTHING){
      // digitalWriteFast(BUSY_LED, HIGH);
    }
    else{
      // digitalWriteFast(BUSY_LED, LOW);
    }

    // here starts our state machine
    switch (currentCommand) {
      // -----------------------------------------------------------------------
      case DO_NOTHING:
        break;
      // -----------------------------------------------------------------------
      case DO_TRIGGER: //
        while (true){
          // instead of storring our trigger state, we use a super simple
          // state machine
          while(TRIG_IN1_LOW){
          }
          // trigger channels as needed

          while(TRIG_IN1_HIGH){
          }

          // trigger channels as needed
        }
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
