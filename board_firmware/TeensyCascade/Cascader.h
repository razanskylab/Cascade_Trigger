// File: Cascader.h
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 24.04.2019 

// Class is used to generate a cascade out of channel subclass

// Changelog:
// 	- moving all timing functions to NOPs
//  - remove all interrupt timers, they induce a lot of jitter

#include "Channel.h"
#include "DacChannel.h"
#include "PinMapping.h"
#include "NOP_CASC.h"
#include <Arduino.h>

#define NCHANNELS 4 // number of laser channels + us pulser channel (excl. DAC)

class Cascader
{
public:
	// constructor
	Cascader()
	{
		// nothing here yet
	}

	// desctructor
	//~Cascader();

	void start_cascade();
	
	// each channel gets an on and an off timepoint, therefoer NCHANNELS * 2 bytes
	void init(
		const uint32_t* timepoints,
		const uint32_t* timepointsDac,
		const uint32_t _nAverages,
		const uint32_t _tAcquire);
	// structure timepoints: onTime0, offTime0, onTime1, offTime1 ... for rising and 
	// falling edges of laser triggers
	// structure timepointsDac: each channel has a corresponding dac polarity switch

private:

	uint16_t nChannels = NCHANNELS; // number of output channels
	Channel chArray[NCHANNELS] = {10, 11, 9, 23}; // arduino pins of channels
	// order: 532, edge, 1064, US pulser
	DacChannel chDac = {12}; // class representing dac channel, argument: pin number
	uint32_t endTime; // time indicating when we are done with cascade [ns]
	uint32_t nAverages; // number of averages acquired at each trigger event
	uint32_t tAcquire; // total acquisition time
};