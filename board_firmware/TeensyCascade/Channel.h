// class representing a single output trigger channel on teensy

#include <Arduino.h>

class Channel
{
public:
	Channel(const unsigned char _pin){
		// get pin number and define it as output pin
		pin = _pin;
		pinMode(pin, OUTPUT);
		digitalWriteFast(pin, LOW);
		// set flags initially to 0
		flagOn = 0;
		flagOff = 0;
	};
	//~Channel();

	void update(const volatile unsigned char& time); // check if we need to set channel high or low
	void reset_flags(); // reset both flags to 0 for next round
	void setOnTime(const unsigned char& _onTime); // sets on time of channel
	void setOffTime(const unsigned char& _offTime); // sets off time of channel
	bool flagDone;

private:
	unsigned char pin; // output pin of teensy
	unsigned char onTime; // micros
	unsigned char offTime; // micros
	volatile bool flagOn; // did we already set trigger high
	volatile bool flagOff; // did we already set trigger low
	bool flagActive; // is 0 if not active and 1 if active
	// to deactivate set onTime == offTime
	
};