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
	DacChannel(const uint8_t _pin);
	DacChannel(const uint8_t _pin, const uint8_t _nTrig);
	
	// class destructor
	~DacChannel();
	
	void update(uint32_t& timeNS);
	void reset_flags();

	void set_trigTime(const float& _trigTime, const uint8_t iEvent);
	float get_trigTime(const uint8_t iEvent) const {return trigTime[iEvent];};

	void set_nTrig(const uint8_t& _nTrig);
	uint8_t get_nTrig() const {return nTrig;};

private:
	float *trigTime = nullptr; // trigger time [ns]
	bool *flagActive = nullptr; // is the channel in use or not
	bool isTrigTimeAlloc = 0;

	uint8_t pin; // output pin of teensy attached to DAC
	uint8_t nTrig = 4; // number of trigger events
	uint32_t iLTrig; // stores last trigger event to avoid multi checking
	bool lastDac; // states if dac was high or low for last trigger event

	void free_mem();
	void alloc_mem();

};