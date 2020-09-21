/* 

	STATUS_LEDS[0] --> waiting for Serial input	
	STATUS_LEDS[1] --> we are in time domain trigger mode
	STATUS_LEDS[2] --> we are in position based trigger mode

	Changelog:
		Switched serial communcation from uint16_t to uint32_t
		Took out 1 second delay in time domain tigger before start

*/
#include "QuadReadout.h"
#include "WaitNS.h"
#include "PinMapping.h"
#include "FreqBasedTrigger.h"

char mode; 
QuadReadout posBasedTrigger;
FreqBasedTrigger freqBasedTrig;

bool lastState = 0;

void setup()
{
	// declare all our status LEDs as output and set to low
	for (unsigned char iLed = 0; iLed < 4; iLed++){
		pinMode(STATUS_LEDS[iLed], OUTPUT);
		digitalWriteFast(STATUS_LEDS[iLed], LOW);
	}

	pinMode(OUTPUT_POSBOARD, OUTPUT);
	digitalWriteFast(OUTPUT_POSBOARD, lastState);

	Serial.begin(115200);
}

void loop(){
	digitalWriteFast(STATUS_LEDS[0], HIGH); // indicate that we are waiting
	while (Serial.available() == 0){
		delayMicroseconds(10); // do nothing but wait
	}
	digitalWriteFast(STATUS_LEDS[0], LOW);

	// read serial input and push to class
	mode = Serial.read();
	/*
		Serial commands supported
		f - frequency domain trigger mode
		s - spatial domain trigger mode
		i - identify device
		r - reset position counter
		p - return current position of hctl counter
	*/
	
	if (mode == 'f'){ // time domain triggering
		// TODO requires implementation
		// declare temporary variables for frequency and position based 
	  // triggering
	  char freqBuffer[4];
	  uint32_t freq;
	  char noShotsBuffer[4];
	  uint32_t noShots;

	  // Read in required frequency and number of shots
	  Serial.readBytes(freqBuffer, 4);
	  Serial.readBytes(noShotsBuffer, 4);
	  freq = (freqBuffer[3] << 24) + (freqBuffer[2] << 16) + 
	  	(freqBuffer[1] << 8) + freqBuffer[0];
	  noShots = (noShotsBuffer[3] << 24) + (noShotsBuffer[2] << 16) + 
	  	(noShotsBuffer[1] << 8) + noShotsBuffer[0];

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
	      delayMicroseconds(10); // wait for starting command

	    input = Serial.read();
	    if (input == 'x'){
	    	Serial.print("r\r"); // let matlab know that we are ready to go
	      freqBasedTrig.start();
	    }
	    else if (input == 'o'){
	      freqBasedTrig.stop();
	    }
	  }
	}else if (mode == 's'){ // spatial domain triggering
		// declare temporary variables for step size and number of steps
  	uint32_t stepSize;
  	uint32_t nTrigger;

	  // Read in required stepsize
	  while(Serial.available() < 8){
	  	// do nothing while 8 bytes of information are flying in
	  }
	  stepSize = // read step size for scan in microm
	   	Serial.read() +
	  	(Serial.read() << 8) + 
	   	(Serial.read() << 16) +
	   	(Serial.read() << 24); 
	  nTrigger = // read number of trigger events
	   	Serial.read() +
	   	(Serial.read() << 8) + 
	   	(Serial.read() << 16) + 
	   	(Serial.read() << 24);

	  // pass information back to MATLAB for doublecheck
	  Serial.print("s: ");
	  Serial.print(stepSize, DEC);
	  Serial.print(" x ");
	  Serial.print(nTrigger, DEC);
	  Serial.print("\r");

	  // pass resolution and number of trigger events to class
	  posBasedTrigger.setXResolution(stepSize);
	  posBasedTrigger.setNTrigger(nTrigger);

	  char input = 'x';
	  while (input != 'o'){
	    // wait for starting command
	    while (Serial.available() == 0)
	      delayMicroseconds(1);
	  
	    input = Serial.read();
	    if (input == 'x'){
	      if (nTrigger == 0){
	      	Serial.print("r\r"); // let matlab know that we are ready to go
	        // posBasedTrigger.start(); // keep running until stop Serial comes in
	      }
	      else{
	      	Serial.print("r\r"); // let matlab know that we are ready to go
	        posBasedTrigger.startN(lastState); // keep going for N steps
	      }
	    } 
	  }
	}else if (mode == 'r'){ // reset position counter to 0
		posBasedTrigger.reset_hctl();
		Serial.print("r\r");
	}else if (mode == 'p'){
		// update position and return through serial 
		posBasedTrigger.update_counter();
		posBasedTrigger.update_counter();
		Serial.print(posBasedTrigger.posCounter);
		Serial.print("\r");
	}else if (mode == 'i'){ // identify yourself as board
		Serial.print("TeensyBasedTrigger\r");
	}else{
		Serial.print("Invalid mode! Returning to wait...\r");		
	}

	// Clear all data in Serial port and return to main application
 	while (Serial.available() > 0)
		Serial.read();

}