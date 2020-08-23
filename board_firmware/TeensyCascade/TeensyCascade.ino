// File: TeensyCascade.ino
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 25.04.2019

#include "Cascader.h"
#include "PinMapping.h"
#include "NOP_CASC.h"

char mode;
char tStart;
Cascader myCascader;
uint16_t nTrigger = 0;

// let led blink for half a second to identify device
void blink_led(){
	digitalWriteFast(LED_BUILTIN, HIGH);
  delay(500);
  digitalWriteFast(LED_BUILTIN, LOW);
  return;
}

// removes all remaining characters from serial port
void clear_serial(){
	while(Serial.available() > 0){
		Serial.read();
		delayMicroseconds(1);
	}
	return;
}

void setup(){
	Serial.begin(115200); // start serial communication
	pinMode(LED_BUILTIN, OUTPUT); // make led output
	blink_led(); // identify device
  pinMode(INPUT_CASCADE, INPUT_PULLUP); // declare input line
  clear_serial();
  pinMode(12, OUTPUT);
}

void loop(){
	while(Serial.available() == 0){ // wait for serial input
		delayMicroseconds(1);
	}

	mode = Serial.read(); // read serial byte
	if (mode == 'i'){ // means identify yourself
		Serial.print("TeensyCascade\r"); // send ID to MATLAB
		blink_led(); // let led blink to indicate which microcontroller we are using
	}else if (mode == 's'){ // start your cascade, stopped any serial input
		bool oldStatus = digitalReadFast(INPUT_CASCADE); 
		clear_serial(); // make sure nothing is in the line before going into loop
		Serial.print("r\r"); // let matlab know that we are ready 
		do{
			// check if new signal change was detected at input pin
		 	if (oldStatus ^ digitalReadFast(INPUT_CASCADE)){
		 		digitalWriteFast(LED_BUILTIN, oldStatus);
		 		oldStatus = !oldStatus; // invert oldStatus
		 		myCascader.start_cascade(); // start event loop
		 	}
		 }while(Serial.available() == 0); 
		 digitalWriteFast(LED_BUILTIN, LOW); // set led to low to identify inactive status

	}else if(mode == 'd'){ // define number of trigger events for cascader
		nTrigger = Serial.read() + (Serial.read() << 8);
		Serial.print("r\r"); // let matlab know that we are ready
	}else if (mode == 'n'){ // means start cascade for n events
		digitalWriteFast(LED_BUILTIN, HIGH);
		bool oldStatus = digitalReadFast(INPUT_CASCADE); // coming from digitalReadFast
		uint16_t iTrigger = 0;
		Serial.print("r\r"); // let matlab know that we are ready
		do{ // trigger because of signal from position board
			if(oldStatus ^ digitalReadFast(INPUT_CASCADE)){	
				oldStatus = !oldStatus; // invert oldStatus
				myCascader.start_cascade(); // start event loop
			 	iTrigger++;
			}
		}while(Serial.available() == 0);

		char trash;
		while(Serial.available() > 0)
			trash = Serial.read(); // delete interrupt

		// let MATLAB know how many trigger events we had
		Serial.print(iTrigger);
		Serial.print("\r");
		digitalWriteFast(LED_BUILTIN, LOW);
	}else if (mode == 'x'){ // initialization
		Serial.print("Init\r");
		// Read timepoints from PC, deactivate channel by setting offtime == ontime
		uint8_t timepoints[NCHANNELS * 2]; // temp array for channel timepoints
		uint8_t timepointsDac[NCHANNELS]; // temp array for dac timepoints
		uint8_t nAverages;
		uint8_t tAcquire;
		uint8_t uTimepointsDac[NCHANNELS];
		for (uint8_t iByte = 0; iByte < (NCHANNELS * 2); iByte++)
			timepoints[iByte] = Serial.read();

		for (uint8_t iByte = 0; iByte < NCHANNELS; iByte++)
			timepointsDac[iByte] = Serial.read();

		// Serial.readBytes(timepoints, NCHANNELS * 2); // read on off timepoints for each channel
		// Serial.readBytes(timepointsDac, NCHANNELS); // read trig timepoints for dac
		nAverages = Serial.read();
		tAcquire = Serial.read();
		// convert tiempointsDac into unsigned
		for (uint8_t iChannel = 0; iChannel < NCHANNELS; iChannel++)
			uTimepointsDac[iChannel] = static_cast<uint8_t>(timepointsDac[iChannel]);

		myCascader.init(timepoints, uTimepointsDac, nAverages, tAcquire); // pass information to cascader class
	}else{ // invalid command
		Serial.print("Invalid mode, returning to wait.\r");
	}

	// clear all serial content before returning to wait
	clear_serial();
}