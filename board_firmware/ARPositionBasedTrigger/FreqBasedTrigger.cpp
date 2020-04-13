#include "FreqBasedTrigger.h"

static void trigger_outer();

// Initialize frequency based trigger
FreqBasedTrigger::freq_based_trigger(void)
{
	pinMode(PIN_LASER_TRIGGER, OUTPUT);
	pinMode(PIN_LED, OUTPUT);
	digitalWriteFast(PIN_LASER_TRIGGER, oldOutput);
}


// Define the trigger frequency
void FreqBasedTrigger::setTrigFreq(const unsigned short& _trigFreq)
{
	// Check for upper and lower limit
	if (_trigFreq == 0){
		trigFreq = 1;
	}else if (_trigFreq >= 100000){
		trigFreq = 100000;
	}else 
	{
	  trigFreq = _trigFreq; 
	}
  
	period = 1000000 / double(trigFreq); // trigger period in µs
}

void FreqBasedTrigger::setNoShots(const unsigned short& _noShots)
{
	noShots = _noShots;
}

// Start trigger if not running
void FreqBasedTrigger::start()
{
	digitalWriteFast(PIN_LED, HIGH);

	// noShots == 0 means we will fire forever, so don't increase the counter
	bool flagRunning = 1;
	unsigned short counter = 0;

	// clear serial input
	clear_serial();

	while (flagRunning)
	{
		triggerSignal();

		if (noShots > 0) // if finite number of shots
		{
			counter++;
			if(counter >= noShots)
				flagRunning = 0;
		}
		else // serial interrupt 
		{
			if (Serial.available() > 0)
				flagRunning = 0;
		}

		if (period < 16383)
			delayMicroseconds(period);
		else
			delay(period / 1000);
	}

	digitalWriteFast(PIN_LED, LOW);

}

// Stop trigger if running
void FreqBasedTrigger::stop()
{
	digitalWriteFast(PIN_LED, LOW);
}

// Generate short pulse which will be later elongated by Nano
// Changed: We now only change the output because nano is sensitive to both rising
// and falling edge
void FreqBasedTrigger::triggerSignal()
{
	oldOutput = !oldOutput;
	digitalWriteFast(PIN_LASER_TRIGGER, oldOutput);
	digitalWriteFast(PIN_LED, oldOutput);
}

void FreqBasedTrigger::clear_serial()
{
	char trash;
	while(Serial.available() > 0)
		trash = Serial.read();
}