#include "DacChannel.h"

#ifndef PIN_LED
  #define PIN_LED 13
#endif

// wait a few clock cycles
#define NOP __asm__ __volatile__ ("nop\n\t") // ~6 ns = one clock cycle
#define WAIT_10_NS NOP; NOP;
#define WAIT_20_NS NOP; NOP; NOP; NOP;
#define WAIT_40_NS WAIT_20_NS; WAIT_20_NS;
#define WAIT_60_NS WAIT_20_NS; WAIT_20_NS; WAIT_20_NS;
#define WAIT_80_NS WAIT_40_NS; WAIT_40_NS;
#define WAIT_100_NS WAIT_80_NS; WAIT_20_NS;
#define WAIT_200_NS WAIT_100_NS; WAIT_100_NS;
#define WAIT_400_NS WAIT_200_NS; WAIT_200_NS;

// checks if we are already ready for next dac trigger, if so switch dac and
// move on to next time point. Exception: all triggers were already applied
void DacChannel::update(const volatile unsigned char& time){

	if ((time >= trigTime[iLTrig]) & (iLTrig < nTrig)){
		digitalWriteFast(pin, HIGH);
		WAIT_100_NS
		digitalWriteFast(pin, LOW);
		iLTrig++; // move to next trigger event
	}

}


// reset class for next trigger cascade
void DacChannel::reset_flags(){
	// set last trigger event to 0 again
	iLTrig = 0;
}

// pass an array of time points to class to initialize it
void DacChannel::setTrigTimes(unsigned char *_trigTimes, unsigned char& _nTrig){
	nTrig = _nTrig;
	trigTime = _trigTimes;
}