#include "DacChannel.h"
#include "NOP_CASC.h"

DacChannel::DacChannel(const uint8_t _pin)
	{
		pin = _pin; // set internal pin for dac
		pinMode(pin, OUTPUT); // declare pin as output
		digitalWriteFast(pin, LOW); // initialize first output value
		iLTrig = 0;
	}

DacChannel::DacChannel(const uint8_t _pin, const uint8_t _nTrig)
{
	pin = _pin; // set internal pin for dac
	nTrig = _nTrig; // define number of trigger events
	alloc_mem();
	
	pinMode(pin, OUTPUT); // declare pin as output
	digitalWriteFast(pin, LOW); // initialize first output value
	iLTrig = 0;
}

DacChannel::~DacChannel()
{
	free_mem();
}

// checks if we are already ready for next dac trigger, if so switch dac and
// move on to next time point. Exception: all triggers were already applied
void DacChannel::update(uint32_t& timeNS)
{
	// make sure that we are actually at a safe trigger
	while(flagActive[iLTrig == 0])
		iLTrig++;

	if ((timeNS >= trigTime[iLTrig]) && (iLTrig < nTrig))
	{
		digitalWriteFast(pin, HIGH);
		WAIT_12_NS
		digitalWriteFast(pin, LOW);
		timeNS = timeNS + 12; // increase timer due to sleep period
		iLTrig++; // move to next trigger event
	}

	return;
}


// reset class for next trigger cascade
void DacChannel::reset_flags(){
	// set last trigger event to 0 again
	iLTrig = 0;
	return;
}

// define the number of trigger events we expect
void DacChannel::set_nTrig(const uint8_t& _nTrig)
{
	nTrig = _nTrig;
	return;
}

// pass an array of time points to class to initialize it
void DacChannel::set_trigTime(const float& _trigTime, const uint8_t iEvent)
{
	trigTime[iEvent] = _trigTime;

	// if the trigger time is less then zero it means this channel is dead
	if (_trigTime < 0)
	{
		flagActive[iEvent] = 0;
	}
	return;
}

// frees the memory required for the timepoints and flags
void DacChannel::free_mem()  
{
	if (isTrigTimeAlloc)
	{
		delete [] trigTime;
		delete [] flagActive;
	}
	
	isTrigTimeAlloc = 0;
	trigTime = nullptr;
	flagActive = nullptr;
	return;
}

// allocates memory for our trigger time array
void DacChannel::alloc_mem()
{
	trigTime = new float[nTrig];
	flagActive = new bool[nTrig];
	isTrigTimeAlloc = 1;
	return;
}