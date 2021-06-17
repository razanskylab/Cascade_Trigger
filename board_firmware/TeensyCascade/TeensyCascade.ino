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
uint32_t nTrigger = 0;

// let led blink for half a second to identify device
void blink_led()
{
	digitalWriteFast(LED_BUILTIN, HIGH);
  delay(500);
  digitalWriteFast(LED_BUILTIN, LOW);
  return;
}

// removes all remaining characters from serial port
void clear_serial()
{
	while(Serial.available() > 0)
	{
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

// serial read of a uint32
uint32_t serialReadUint32()
{
	char uintBuffer[4];
	Serial.readBytes(uintBuffer, 4);
	uint32_t value = (uintBuffer[3] << 24) + (uintBuffer[2] << 16) + 
	  	(uintBuffer[1] << 8) + uintBuffer[0];
	return value;
}

// serial read of a uint16
uint32_t serialReadUint16()
{
	char uintBuffer[2];
	Serial.readBytes(uintBuffer, 2);
	uint16_t value = (uintBuffer[1] << 8) + uintBuffer[0];
	return value;
}

void loop()
{
	while(Serial.available() == 0) // wait for serial input
		delayMicroseconds(1);

	mode = Serial.read(); // read serial byte
	if (mode == 'i') // identify device for COM port detection
	{
		Serial.print("TeensyCascade\r"); // send ID to MATLAB
		blink_led(); // blink LED to indicate which microcontroller we are using
	}
	else if (mode == 's') // start your cascade, stopped any serial input
	{
		bool oldStatus = digitalReadFast(INPUT_CASCADE); 
		clear_serial(); // make sure nothing is in the line before going into loop
		Serial.print("r\r"); // let matlab know that we are ready 
		do
		{
			// check if new signal change was detected at input pin
		 	if (oldStatus ^ digitalReadFast(INPUT_CASCADE))
		 	{
		 		digitalWriteFast(LED_BUILTIN, oldStatus);
		 		oldStatus = !oldStatus; // invert oldStatus
		 		myCascader.start_cascade(); // start event loop
		 	}
		 }while(Serial.available() == 0); 
		 digitalWriteFast(LED_BUILTIN, LOW); // set led to low to identify inactive status
	}
	else if(mode == 'd') // define number of trigger events for cascader
	{ 
		nTrigger = serialReadUint32();
		Serial.print("r\r"); // let matlab know that we are ready
	}
	else if (mode == 'n') // means start cascade until serial interrupt
	{ 
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
	}
	else if (mode == 'x') // initialization
	{ 
		Serial.print("Init\r");
		// Read timepoints from PC, deactivate channel by setting offtime == ontime
		uint32_t timepoints[NCHANNELS * 2]; // temp array for channel timepoints
		uint32_t timepointsDac[NCHANNELS]; // temp array for dac timepoints
		uint32_t nAverages;
		uint32_t tAcquire;
		uint32_t uTimepointsDac[NCHANNELS];

		for (uint8_t iByte = 0; iByte < (NCHANNELS * 2); iByte++)
			timepoints[iByte] = serialReadUint32();

		for (uint8_t iByte = 0; iByte < NCHANNELS; iByte++)
			timepointsDac[iByte] = serialReadUint32(); // convert into 100 NS steps

		nAverages = serialReadUint32();
		tAcquire = serialReadUint32(); // convert into 100 NS steps
		myCascader.init(&timepoints[0], &timepointsDac[0], nAverages, tAcquire); // pass information to cascader class
	}
	else // invalid command
	{
		Serial.print("Invalid mode, returning to wait.\r");
	}

	clear_serial(); // clear all serial content before returning to wait
}