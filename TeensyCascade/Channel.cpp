#include "Channel.h"

// check if we either need to set channel low or high
void Channel::update(const volatile unsigned char& time){
	if (flagActive){
		if (!flagOn){
			if (time >= onTime){
				digitalWriteFast(pin, HIGH);
				flagOn = 1;
			}
		}

		if (!flagOff){
			if (time >= offTime){
				digitalWriteFast(pin, LOW);
				flagOff = 1;
				flagDone = 1;
			}
		}
	}
}

// resets both flags back to 0 for next trigger round
void Channel::reset_flags(){
	flagOff = 0;
	flagOn = 0;
	if (flagActive)
		flagDone = 0;
	else
		flagDone = 1;
}

void Channel::setOnTime(const unsigned char& _onTime){
	onTime = _onTime;
}

void Channel::setOffTime(const unsigned char& _offTime){
	offTime = _offTime;

	// check if offtime is the same as ontime and set flag correspondigly
	if (onTime == offTime){
		flagActive = 0;
		flagDone = 1;
	}else{
		flagActive = 1;
		flagDone = 0;
	}
}