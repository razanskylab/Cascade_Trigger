// File: TeensyCascade.ino
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 25.04.2019

#include "Cascader.h"

#ifndef PIN_LED
  #define PIN_LED 13
#endif

#ifndef PIN_INPUT
	#define PIN_INPUT 22 // corresponds to pin 2 of position counter teensy
#endif

char mode;
char tStart;
Cascader myCascader;

// let led blink for half a second to identify device
void blink_led(){
	digitalWriteFast(PIN_LED, HIGH);
  	delay(500);
  	digitalWriteFast(PIN_LED, LOW);
}

// removes all remaining characters from serial port
void clear_serial(){
	while(Serial.available() > 0){
		Serial.read();
		delay(10);
	}
}

void setup(){
	Serial.begin(115200); // start serial communication
	pinMode(PIN_LED, OUTPUT); // make led output
	blink_led(); // identify device
  	pinMode(PIN_INPUT, INPUT); // declare input line
  	clear_serial();
}


void loop(){

	// wait for serial input
	while(Serial.available() == 0)
		delayMicroseconds(10);

	// read serial byte
	mode = Serial.read();
	if (mode == 'i'){ // means identify yourself

		// send ID followed by termiantor to matlab
		Serial.print("TeensyCascade\r");
		
		// let led blink to indicate which microcontroller we are using
		blink_led();

	}else if (mode == 's') // means start your cascade, can be stopped using any serial command
	{
		Serial.print("Start\r"); 
		bool oldStatus = digitalReadFast(PIN_INPUT);
		digitalWriteFast(PIN_LED, HIGH); // set led to high to identify active status
		clear_serial(); // make sure nothing is in the line before going into loop
		do
		 {
		 	// check if new signal change was detected at input pin
		 	if (oldStatus ^ digitalReadFast(PIN_INPUT)){
		 		oldStatus = !oldStatus; // invert oldStatus
		 		myCascader.start_cascade(); // start event loop
		 	}

		 } while (Serial.available() == 0); 
		 digitalWriteFast(PIN_LED, LOW); // set led to low to identify inactive status

	}else if (mode == 'x'){
		Serial.print("Init\r");
		// Read timepoints from PC, deactivate channel by setting offtime == ontime
		char timepoints[NCHANNELS * 2]; // temp array for channel timepoints
		char timepointsDac[NCHANNELS]; // temp array for dac timepoints
		char nAverages;
		char tAcquire;
		unsigned char uTimepointsDac[NCHANNELS];
		Serial.readBytes(timepoints, NCHANNELS * 2); // read on off timepoints for each channel
		Serial.readBytes(timepointsDac, NCHANNELS); // read trig timepoints for dac
		nAverages = Serial.read();
		tAcquire = Serial.read();
		// convert tiempointsDac into unsigned
		for (unsigned char iChannel = 0; iChannel < NCHANNELS; iChannel++)
			uTimepointsDac[iChannel] = static_cast<unsigned char>(timepointsDac[iChannel]);

		myCascader.init(timepoints, uTimepointsDac, nAverages, tAcquire); // pass information to cascader class
	}else // invalid command
	{
		Serial.print("Invalid mode, returning to wait.\r");
	}

	// clear all serial content before returning to wait
	clear_serial();
}