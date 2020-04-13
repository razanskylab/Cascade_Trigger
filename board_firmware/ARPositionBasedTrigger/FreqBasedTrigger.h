#ifndef FREQ_BASED_TRIGGER_H
#define FREQ_BASED_TRIGGER_H

#include "Arduino.h"

#ifndef PIN_LASER_TRIGGER
	#define PIN_LASER_TRIGGER 3
#endif

#ifndef PIN_LED
  #define PIN_LED 13
#endif


class FreqBasedTrigger{

private:
	
	unsigned short trigFreq = 10;
	unsigned short noShots = 0; 

	unsigned int period = 100000; // in microseconds
	bool isRunning = 0;	
	
	// if 0, then it will run forever
	// otherwise this value defines the number of shots after which it automatically stops  
	bool oldOutput = 0;

public:
	FreqBasedTrigger();
	void triggerSignal();
	void setTrigFreq(const unsigned short& _trigFreq);
	void setNoShots(const unsigned short& _noShots);
	void start();
	void stop();
	void clear_serial();
};

extern FreqBasedTrigger FBTx; 

#endif	
