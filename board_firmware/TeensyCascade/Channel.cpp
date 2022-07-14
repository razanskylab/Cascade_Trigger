#include "Channel.h"

// check if we either need to set channel low or high
void Channel::update(const uint32_t& time){
	if (flagActive)
	{
		if (!flagOn)
		{
			if (time >= onTime)
			{
				digitalWrite(pin, HIGH);
				flagOn = 1;
			}
		}
		else	if (!flagOff)
		{
			if (time >= offTime)
			{
				digitalWrite(pin, LOW);
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

// defines the onset time of the trigger channel
void Channel::set_onTime(const float& _onTime)
{
	onTime = _onTime;
	return;
}

// defines the off time of the trigger channel
void Channel::set_offTime(const float& _offTime)
{
	offTime = _offTime;

	// check if offtime is the same as ontime and set flag correspondigly
	if (onTime == offTime)
	{
		flagActive = 0;
		flagDone = 1;
	}
	else
	{
		flagActive = 1;
		flagDone = 0;
	}
	return;
}