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
#include "SerialNumbers.h"
#include "NOP_CASC.h"
#include <Arduino.h>

class Cascader
{
public:
	// constructor
	Cascader();

	// desctructor
	//~Cascader();

	void setup();
	void operate();
	
	// each channel gets an on and an off timepoint, therefoer NCHANNELS * 2 bytes
	// structure timepoints: onTime0, offTime0, onTime1, offTime1 ... for rising and 
	// falling edges of laser triggers
	// structure timepointsDac: each channel has a corresponding dac polarity switch

private:

	const uint8_t nChannels = 4; // number of output channels
	Channel chArray[4] = {10, 11, 9, 23}; // arduino pins of channels
	// order: 532, edge, 1064, US pulser
	DacChannel chDac = {12, 4}; // dac channel, argument: pin numbe; on board Connector SMA 1, pin 12 of arduino, correct; 4 means 4 _nTrig
	uint32_t endTime; // time indicating when we are done with cascade [ns]
	float tAcquire = 6.0f; // total acquisition time inn micros
	uint32_t nAverages = 1; // number of averages acquired at each trigger event
	uint32_t nShots = 0; // number of shots we want to do
	uint8_t inputPin = 0; // maps to labels on board with offset of 1 
	// 0 maps to labels on board with Trigger IN of 1 --> Pin 16 -->Trigger In 1
	// 1 maps to labels on board with Trigger IN of 2 --> Pin 17 -->Trigger In 2
	// 2 maps to labels on board with Trigger IN of 3 --> Pin 18 -->Trigger In 3
	// 3 maps to labels on board with Trigger IN of 4 --> Pin 19 -->Trigger In 4
	const uint8_t INPUT_PINS[4] = {16, 17, 18, 19};
	uint8_t inputTrigType = TRIG_BOTH; // rising, falling, or both edges
	bool oldStatus = 0;
	uint16_t iTrigger = 0;

	// pin definitios
	const uint8_t LED_STATUS = LED_BUILTIN; 

	// here goes the full communication protocol
	const uint8_t IDENTIFY = 00;

	// commands defining values
	const uint8_t SET_AVERAGES = 11;
	const uint8_t SET_NSHOTS = 12;
	const uint8_t SET_TACQUIRE = 13;
	const uint8_t SET_TON = 14; // defines the start timepoint for a laser
	const uint8_t SET_TOFF = 15; // defines the start timepoint for a laser
	const uint8_t SET_TDAC = 16; // receive NCHANNELS floats for dac timepoints
	const uint8_t SET_INPUT_PIN = 17; // define cascader input (1, 2, 3)
	const uint8_t SET_TRIGTYPE = 18;

	// commands requesing vaSlues
	const uint8_t GET_AVERAGES = 21;
	const uint8_t GET_NSHOTS = 22;
	const uint8_t GET_TACQUIRE = 23;
	const uint8_t GET_TON = 24;
	const uint8_t GET_TOFF = 25;
	const uint8_t GET_TDAC = 26; // returns NCHANNELS floats for dac timepoints
	const uint8_t GET_INPUT_PIN = 27; // send cascader input back (1, 2, or 3)
	const uint8_t GET_TRIGTYPE = 28;

	const uint8_t START_CASCADE = 31; // starts the laser cascade
	const uint8_t STOP_CASCADE = 32; // stops the laser cascade

	const uint8_t TRIG_RISING = 81; // trigger on rising edge
	const uint8_t TRIG_FALLING = 82; // trigger on falling edge
	const uint8_t TRIG_BOTH = 83; // trigger on rising and falling edge

	const uint8_t OK = 91;
	const uint8_t WARNING = 92;
	const uint8_t ERROR = 93;
	const uint8_t UNKNOWN_COMMAND = 94;
	
	const uint16_t IDENTIFIER = 75; // identifier that we are the TriggerBoard

	// some private functions
	void wait_for_serial(const uint16_t nSerial);
	void identify();
	void check_trigger();
	void cascade(); // perfroms a single cascade
	void start_ncascade(); // version of cascader running for n events
	void start_scascade(); // version of cascader waiting for serial interupt
	void define_input_pin(const uint8_t iChannel);

};