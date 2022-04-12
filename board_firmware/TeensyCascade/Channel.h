// File: Channel.h
// Auhtor: Urs Hofmann
// Mail: hofmannu@ethz.ch
// Date: 22.02.2021

// class representing a single output trigger channel on teensy

// Changelog
// 	- move all timing related stuff to the ns range
// 	- all timepoints need uint32 now

#include <Arduino.h>

class Channel
{
public:
	// class constructor
	Channel(const uint8_t _pin){
		// get pin number and define it as output pin
		pin = _pin;
		pinMode(pin, OUTPUT); // define arduino pin to output
		digitalWriteFast(pin, LOW); // set initially to low
		flagOn = 0;
		flagOff = 0;
	};

	//~Channel();

	void update(const uint32_t& time); // check if we need to set channel high or low
	void reset_flags(); // reset both flags to 0 for next round
	void set_onTime(const float& _onTime); // sets on time of channel
	void set_offTime(const float& _offTime); // sets off time of channel

	float get_onTime() const {return onTime;};
	float get_offTime() const {return offTime;};

	bool flagDone;

private:
	uint8_t pin; // output pin of teensy
	float onTime; // timepoint when trigger should be put to high [ns]
	float offTime; // timepoint when trigger should be put to low [ns]
	volatile bool flagOn; // did we already set trigger high
	volatile bool flagOff; // did we already set trigger low
	bool flagActive; // is 0 if not active and 1 if active
	// to deactivate set onTime == offTime
};