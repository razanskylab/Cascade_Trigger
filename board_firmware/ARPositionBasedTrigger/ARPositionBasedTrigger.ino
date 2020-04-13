/* 

	STATUS_LEDS[0] --> waiting for Serial input	
	STATUS_LEDS[1] --> we are in time domain trigger mode
	STATUS_LEDS[2] --> we are in position based trigger mode

*/
#include "QuadReadout.h"
#include "WaitNS.h"
#include "PinMapping.h"
#include "FreqBasedTrigger.h"

char mode; 
QuadReadout posBasedTrigger;
FreqBasedTrigger freqBasedTrig;

void setup()
{
	// declare all our status LEDs as output and set to low
	for (unsigned char iLed = 0; iLed < 4; iLed++){
		pinMode(STATUS_LEDS[iLed], OUTPUT);
		digitalWriteFast(STATUS_LEDS[iLed], LOW);
	}

	Serial.begin(115200);
}

void loop(){
	digitalWriteFast(STATUS_LEDS[0], HIGH);
	while (Serial.available() == 0){
		delayMicroseconds(10);
		// do nothing but wait for serial input
	};
	digitalWriteFast(STATUS_LEDS[0], LOW);

	// read serial input and push to class
	mode = Serial.read();
	

	if (mode == 'f'){ // time domain triggering
		// TODO requires implementation
		// declare temporary variables for frequency and position based 
	    // triggering
	    char freqBuffer[2];
	    unsigned short freq;
	    char noShotsBuffer[2];
	    unsigned short noShots;

	    // Read in required frequency and number of shots
	    Serial.readBytes(freqBuffer, 2);
	    Serial.readBytes(noShotsBuffer, 2);
	    freq = (freqBuffer[1]<<8)+freqBuffer[0];
	    noShots = (noShotsBuffer[1]<<8)+noShotsBuffer[0];

	    // Let computer know what we understood
	    Serial.print("f: ");
	    Serial.print(freq);
	    Serial.print(" x ");
	    Serial.print(noShots);
	    Serial.print("\r");
	    freqBasedTrig.setTrigFreq(freq);
	    freqBasedTrig.setNoShots(noShots);
	    
	    char input = 'x';
	    while (input != 'o')
	    {
	      while (Serial.available() == 0)
	      {
	        // wait for starting command
	        delay(1);
	      }
	      input = Serial.read();
	      if (input == 'x')
	        freqBasedTrig.start();
	      else if (input == 'o')
	        freqBasedTrig.stop();
	    }
	}
	else if (mode == 's'){ // spatial domain triggering
		// TODO requires implementation
		// declare temporary variables for step size and number of steps
	    char stepBuffer[2];
	    unsigned short stepSize;
	    char nTriggerBuffer[2];
	    unsigned short nTrigger;

	    // Read in required stepsize
	    Serial.readBytes(stepBuffer, 2);
	    Serial.readBytes(nTriggerBuffer, 2);
	    stepSize = (stepBuffer[1]<<8) + stepBuffer[0];
	    nTrigger = (nTriggerBuffer[1]<<8) + nTriggerBuffer[0];

	    Serial.print("s: ");
	    Serial.print(stepSize);
	    Serial.print(" micron x ");
	    Serial.print(nTrigger);
	    Serial.print("\r");
	    posBasedTrigger.setXResolution(stepSize);
	    posBasedTrigger.setNTrigger(nTrigger);

	    char input = 'x';
	    while (input != 'o'){
	      // wait for starting command
	      while (Serial.available() == 0)
	        delayMicroseconds(10);
	      
	      input = Serial.read();
	      if (input == 'x')
	        if (nTrigger == 0)
	          posBasedTrigger.start(); // keep running until stop Serial comes in
	        else
	          posBasedTrigger.startN(); // keep going for N steps
	    }
	}
	else if (mode == 'i'){ // identify yourself as board
		Serial.print("TeensyBasedTrigger\r");
	}
	else{
		Serial.print("Invalid mode! Returning to wait...\r");		
	}

	// Clear all data in Serial port and return to main application
 	while (Serial.available() > 0)
	{
		Serial.read();
	}

}