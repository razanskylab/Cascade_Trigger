// File: TeensyCascade.ino
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 25.04.2019

// Changlog:
// 	- added second input line coming from separated position based trigger
// 	- add option for changing polarity on stage channel

#include "Cascader.h"
#include "PinMapping.h"
#include "NOP_CASC.h"
#include "serial_comm.h"

char mode;
char tStart;
Cascader myCascader;
bool oldStatus;
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

// uint8_t inputPin = 17;
uint8_t inputPin = 16;
bool triggerType = 1; // default trigger type is 1, means rising edge ONLY

void setup(){
	Serial.begin(115200); // start serial communication
	pinMode(LED_BUILTIN, OUTPUT); // make led output
	blink_led(); // identify device
  
  pinMode(inputPin, INPUT); // declare input line
  oldStatus = digitalReadFast(inputPin);

  clear_serial();
  pinMode(12, OUTPUT);
}

void loop()
{
	while(Serial.available() == 0) // wait for serial input
		delayMicroseconds(1);

	// overview modes:
	// 		i - identify by sending name back and blinking
	// 		j - identify by only sending name back
	//		s - start teensy cascade until serial interupt
	// 		n - start teensy cascade for n events
	// 		x - initialize cascade settings
	// 		o - stop cascade
	// 		p - define input pin of cascade
	//    b - set trigger type to both rising and falling edge

	mode = Serial.read(); // read serial byte
	if (mode == 'i') // identify device for COM port detection
	{
		Serial.write("TeensyCascade"); // send ID to MATLAB
		blink_led(); // blink LED to indicate which microcontroller we are using
		Serial.write("r");
	}
	else if (mode == 'j') // identify without blink
	{
		Serial.write("TeensyCascade"); // send ID to MATLAB
		Serial.write("r");
	}
	else if (mode == 'p') // redefine the input pin for the cascader
	{
		inputPin = read_uint8();
		pinMode(inputPin, INPUT);
		oldStatus = digitalReadFast(inputPin);
		send_uint8(inputPin);
		Serial.write("r");
	}
	else if (mode == 'b')
	{
		triggerType = 0;
		// handshake with Matlab to confirm setting trigger type command
		Serial.write("r");
	}
	else if (mode == 's') // start your cascade, stopped any serial input
	{
		oldStatus = digitalReadFast(inputPin); 
		uint32_t iTrigger = 0;
		Serial.write("r"); // let matlab know that we are ready 
		do
		{
			if (triggerType)
			{
				// We ONLY trigger upon rising edge
				if (digitalReadFast(inputPin))
				{	
					myCascader.start_cascade(); // start event loop
			 		iTrigger++;
			 		while (digitalReadFast(inputPin))
			 		{
			 			// do nothing but wait for signal to come down
			 		}
				}
			}
			else
			{
				// check if new signal change was detected at input pin
				if (oldStatus ^ digitalReadFast(inputPin))
				{
					oldStatus = !oldStatus; // invert oldStatus
					myCascader.start_cascade(); // start event loop
					iTrigger++;
				}
			}
		}while(Serial.available() == 0);

		// check if the command we reveived was stop
		const char recCmd = Serial.read();
		if (recCmd == 'o')
		{
			Serial.write("r"); // let matlab know that we will stop
			send_uint32(iTrigger);
			Serial.write("r"); // let matlab know that we will stop		
		}
		else
		{
			Serial.write("e");
		}
		digitalWriteFast(LED_BUILTIN, LOW); // set led to low to identify inactive status
	}
	else if(mode == 'd') // define number of trigger events for cascader
	{ 
		nTrigger = read_uint32();
		Serial.write("r"); // let matlab know that we are ready
	}
	else if (mode == 'n') // means start cascade for n events
	{ 

		Serial.write("r");
		digitalWriteFast(LED_BUILTIN, HIGH);
		oldStatus = digitalReadFast(inputPin); // coming from digitalReadFast
		
		uint32_t nTrigger = read_uint32();
		uint32_t iTrigger = 0;
		send_uint32(nTrigger);
		Serial.write("r"); // let matlab know that we are ready
		
		do{
			if (triggerType)
			{
				// We ONLY trigger upon rising edge
				if (digitalReadFast(inputPin))
				{	
					myCascader.start_cascade(); // start event loop
			 		iTrigger++;
			 		while (digitalReadFast(inputPin))
			 		{
			 			// do nothing but wait for signal to come down
			 		}
				}
			}
			else
			{
				// check if new signal change was detected at input pin
				if (oldStatus ^ digitalReadFast(inputPin))
				{
					oldStatus = !oldStatus; // invert oldStatus
					myCascader.start_cascade(); // start event loop
					iTrigger++;
				}
			}
		}while(iTrigger < nTrigger);

		send_uint32(iTrigger); // let MATLAB know how many trigger events we had
		Serial.write("r"); // let matlab know that we will stop

		digitalWriteFast(LED_BUILTIN, LOW);
	}
	else if (mode == 'x') // initialization
	{ 
		Serial.write("r");
		// Read timepoints from PC, deactivate channel by setting offtime == ontime
		uint32_t timepoints[NCHANNELS * 2]; // temp array for channel timepoints
		uint32_t timepointsDac[NCHANNELS]; // temp array for dac timepoints
		uint32_t nAverages;
		uint32_t tAcquire;
		// uint32_t imepointsDac[NCHANNELS];

		for (uint8_t iByte = 0; iByte < (NCHANNELS * 2); iByte++)
			timepoints[iByte] = read_uint32();

		for (uint8_t iByte = 0; iByte < NCHANNELS; iByte++)
			timepointsDac[iByte] = read_uint32(); // convert into 100 NS steps

		nAverages = read_uint32();
		tAcquire = read_uint32(); // convert into 100 NS steps
		Serial.write("r");

		myCascader.init(&timepoints[0], &timepointsDac[0], nAverages, tAcquire); // pass information to cascader class
	}
	else if (mode == 'o') // invalid command
	{
		Serial.write("w");
	}
	else
	{
		Serial.write("e");
	}
  // clear_serial(); // clear all serial content before returning to wait
}
