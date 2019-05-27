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
	DacChannel(const unsigned char _pin){
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

	void update(const volatile unsigned char& time);
	void reset_flags();
	void setTrigTimes(unsigned char *_trigTimes, unsigned char& _nTrig);

private:
	unsigned char pin; // output pin of teensy attached to DAC
	unsigned char nTrig; // number of trigger events
	unsigned char *trigTime; // trigger time in micros
	unsigned char iLTrig; // stores last trigger event to avoid multi checking
	bool lastDac; // states if dac was high or low for last trigger event

};