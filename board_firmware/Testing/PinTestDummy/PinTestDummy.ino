/*
	File: PiNTestDummy.ino
	Author: Urs Hofmann
	Mail: hofmannu@biomed.ee.ethz.ch
	Date: 14.04.2020

	Description: Dummy test file to identify output channels of cascade board
*/

#ifndef OUTPUT_PIN
	#define OUTPUT_PIN 19
#endif

#define LED_PIN 13

// SMA 1: teensy pin 12
// SMA 2: teensy pin 11
// SMA 3: teensy pin 10
// SMA 4: 
// SMA 5:  

void setup(){
	pinMode(OUTPUT_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
};


void loop()
{
	digitalWriteFast(OUTPUT_PIN, 1);
	digitalWriteFast(LED_PIN, 1);
	delay(500);

	digitalWriteFast(OUTPUT_PIN, 0);
	digitalWriteFast(LED_PIN, 0);
	
	delay(500);
};