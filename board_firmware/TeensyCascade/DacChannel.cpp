#include "DacChannel.h"
#include "NOP_CASC.h"

// checks if we are already ready for next dac trigger, if so switch dac and
// move on to next time point. Exception: all triggers were already applied
void DacChannel::update(uint32_t& timeNS){

	if ((timeNS >= trigTime[iLTrig]) & (iLTrig < nTrig)){
		digitalWriteFast(pin, HIGH);
		WAIT_12_NS
		digitalWriteFast(pin, LOW);
		iLTrig++; // move to next trigger event
		timeNS = timeNS + 12; // increase timer due to sleep period
	}
	return;
}


// reset class for next trigger cascade
void DacChannel::reset_flags(){
	// set last trigger event to 0 again
	iLTrig = 0;
	return;
}

// pass an array of time points to class to initialize it
void DacChannel::setTrigTimes(uint32_t* _trigTimes, const uint32_t _nTrig){
	nTrig = _nTrig;
	trigTime = _trigTimes;
	return;
}