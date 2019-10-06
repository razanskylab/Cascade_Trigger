#include "..\lib\trigger_lib.cpp"

// things to discuss with CHEN:
// only integer us values allowed for trigger period -> only fixed PRFs possible

// todo list
// do_trigger() triggers on rising and falling flank FIXME -> change this or
// make it optional!!!

// do_trigger() has fixxed 1 us trigger length, make it at least variable


// 180


TeensyTrigger MyTrig;

void setup() {
  setup_serial();
  MyTrig.setup_io_pins();
}

void loop() {
  while(true){ // loop is slower than while(true)
    // here starts our state machine
    MyTrig.chen_stand_alone_trigger();
    switch (MyTrig.currentCommand) {
      // -----------------------------------------------------------------------
      case DO_NOTHING:
        break;

      // -----------------------------------------------------------------------
      case SET_TRIGGER_CH:
        MyTrig.set_trigger_channel();
        break;

      // -----------------------------------------------------------------------
      case EXT_TRIGGER:
        MyTrig.external_trigger();
        break;

      // -----------------------------------------------------------------------
      case ENABLE_INT_TRIGGER:
        MyTrig.chen_stand_alone_trigger();
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
    } // switch
  } // while
} // loop()


// TRIG_OUT_PORT |= (1UL << 1-1); // set 0st bit = first port
// TRIG_OUT_PORT &= ~(1UL << 3-1); // clear 2nd bit = third port
// TRIG_OUT_PORT ^= (1UL << 5-1); // toggle 4th bit = 5th port
