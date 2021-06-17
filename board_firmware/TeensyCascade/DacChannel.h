// File: DacChannel.h
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 29.04.2019

// Description: Class representing and triggering the DAC
// channel based on an up down pattern making use of both
// rising and falling edge 

#include <Arduino.h>

class DacChannel{

public:
	// class constructor
	DacChannel(const uint8_t _pin)
	{
		pin = _pin; // set internal pin for dac
		pinMode(pin, OUTPUT); // declare pin as output
		digitalWriteFast(pin, LOW); // initialize first output value
		iLTrig = 0;
	}

	// class destructor
	// free memory of dynamic trigTime array
	~DacChannel(){
		delete [] trigTime;
	}

	void update(uint32_t& timeNS);
	void reset_flags();
	void setTrigTimes(uint32_t *_trigTimes, const uint32_t _nTrig);

private:
	uint8_t pin; // output pin of teensy attached to DAC
	uint32_t nTrig; // number of trigger events
	uint32_t *trigTime; // trigger time [ns]
	uint32_t iLTrig; // stores last trigger event to avoid multi checking
	bool lastDac; // states if dac was high or low for last trigger event

};