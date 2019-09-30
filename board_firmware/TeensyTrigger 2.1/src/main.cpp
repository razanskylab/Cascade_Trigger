#include "..\lib\trigger_lib.cpp"

// todo list
// do_trigger() triggers on rising and falling flank FIXME -> change this or
// make it optional!!!

// do_trigger() has fixxed 1 us trigger length, make it at least variable


TeensyTrigger MyTrig;

void setup() {
  setup_serial();
  MyTrig.setup_io_pins();
}

void loop() {
  while(true){ // loop is slower than while(true)

    delay(500);
    LED_PORT = 0b00000000; // enable LEDS based on current trigger mode
    delay(500);
    LED_PORT = 0b11111111; // enable LEDS based on current trigger mode

    if (MyTrig.check_for_serial_command())
      MyTrig.execute_serial_command();
  }
}
