#ifndef OUTPUT_PIN
	#define OUTPUT_PIN 10
#endif

// SMA 1: teensy pin 12
// SMA 2: teensy pin 11
// SMA 3: teensy pin 10

void setup(){
	pinMode(OUTPUT_PIN, OUTPUT);
};


void loop(){
	digitalWriteFast(OUTPUT_PIN, 1);
	delayMicroseconds(100);
	digitalWriteFast(OUTPUT_PIN, 0);
	delayMicroseconds(100);
};