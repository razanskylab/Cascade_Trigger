#include "DacChannel.h"
#include "NOP_CASC.h"

// checks if we are already ready for next dac trigger, if so switch dac and
// move on to next time point. Exception: all triggers were already applied
void DacChannel::update(const volatile unsigned char& time){

	if ((time >= trigTime[iLTrig]) & (iLTrig < nTrig)){
		digitalWriteFast(pin, HIGH);
		WAIT_400_NS
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