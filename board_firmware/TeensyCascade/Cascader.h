// File: Cascader.h
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 24.04.2019 

// Class is used to generate a cascade out of channel subclass

#include "Channel.h"
#include "DacChannel.h"
#include "PinMapping.h"
#include <Arduino.h>

#define NCHANNELS 3 // number of laser channels + us pulser channel (excl. DAC)

class Cascader
{
public:
	// constructor
	Cascader(){
		//myTimer.priority(0);
	}

	// desctructor
	//~Cascader();

	void start_cascade();
	
	// each channel gets an on and an off timepoint, therefoer NCHANNELS * 2 bytes
	void init(
		const uint8_t timepoints[NCHANNELS * 2],
		uint8_t timepointsDac[NCHANNELS],
		const uint8_t _nAverages,
		const uint8_t _tAcquire);
	// structure timepoints: onTime0, offTime0, onTime1, offTime1 ... for rising and 
	// falling edges of laser triggers
	// structure timepointsDac: each channel has a corresponding dac polarity switch

private:

	// channels represent each 
	uint8_t nChannels = NCHANNELS;
	Channel chArray[NCHANNELS] = {10, 11, 9}; // arduino pins of channels
	// order: 532, edge, 1064, US pulser
	DacChannel chDac = {12}; // class representing dac channel, argument: pin number
	uint8_t endTime; // time indicating when we are done with cascade [micros]
	IntervalTimer myTimer; // timer used instead of elapsedMicros since more accurate
	uint8_t nAverages;
	uint8_t tAcquire;
};