#include "Cascader.h"


// just an empty constructor for now
Cascader::Cascader()
{

}

// device setup function
void Cascader::setup()
{
	Serial.begin(115200); // start serial communication
	pinMode(LED_STATUS, OUTPUT); // make led output
	pinMode(INPUT_PINS[inputPin], INPUT); // declare input line
}

// wait until a certain amount of serial bytes arrived
void Cascader::wait_for_serial(const uint16_t nSerial)
{
	while (Serial.available() < nSerial)
	{
		delayMicroseconds(1);
	}
	return;
}

// ifentifies the board with a LED blink
void Cascader::identify()
{
	SerialNumbers::send_uint16(IDENTIFIER);
	digitalWrite(LED_STATUS, HIGH);
	delay(250);
	digitalWrite(LED_STATUS, LOW);
	return;
}

void Cascader::operate()
{
	wait_for_serial(1);

	uint8_t mode = SerialNumbers::read_uint8(); // read serial byte
	if (mode == IDENTIFY) // identify device for COM port detection
	{
		identify();
		SerialNumbers::send_uint8(OK);
	}
	// define number of averages acquired at each trigger event
	else if (mode == SET_AVERAGES)
	{
		nAverages = SerialNumbers::read_uint32();
		SerialNumbers::send_uint32(nAverages);
		SerialNumbers::send_uint8(OK);
	}
	// return number of averages currently defined on microcontroller
	else if (mode == GET_AVERAGES)
	{
		SerialNumbers::send_uint32(nAverages);
		SerialNumbers::send_uint8(OK);
	}
	// define total number of shots we want to fire
	else if (mode == SET_NSHOTS)
	{
		nShots = SerialNumbers::read_uint32();
		SerialNumbers::send_uint32(nShots);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_NSHOTS)
	{
		SerialNumbers::send_uint32(nShots);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == SET_TACQUIRE)
	{
		tAcquire = SerialNumbers::read_float();
		SerialNumbers::send_float(tAcquire);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_TACQUIRE)
	{
		SerialNumbers::send_float(tAcquire);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_TON)
	{	
		const uint8_t idLaser = SerialNumbers::read_uint8();
		SerialNumbers::send_uint8(idLaser);
		SerialNumbers::send_float(chArray[idLaser].get_onTime());
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == SET_TON)
	{
		// receive ID and trigger timepoint
		const uint8_t idLaser = SerialNumbers::read_uint8();
		const float tTrig = SerialNumbers::read_float();

		chArray[idLaser].set_onTime(tTrig);

		// send back confirmation
		SerialNumbers::send_uint8(idLaser);
		SerialNumbers::send_float(chArray[idLaser].get_onTime());
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_TOFF)
	{	
		const uint8_t idLaser = SerialNumbers::read_uint8();
		SerialNumbers::send_uint8(idLaser);
		SerialNumbers::send_float(chArray[idLaser].get_offTime());
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == SET_TOFF)
	{
		// receive ID and trigger timepoint
		const uint8_t idLaser = SerialNumbers::read_uint8();
		const float tTrig = SerialNumbers::read_float();

		chArray[idLaser].set_offTime(tTrig);

		// send back confirmation
		SerialNumbers::send_uint8(idLaser);
		SerialNumbers::send_float(chArray[idLaser].get_offTime());
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == SET_TDAC)
	{
		for (uint8_t iChannel = 0; iChannel < nChannels; iChannel++)
		{
			const float tempVal = SerialNumbers::read_float();
			chDac.set_trigTime(tempVal , iChannel);
			SerialNumbers::send_float(chDac.get_trigTime(iChannel));
		}
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_TDAC)
	{
		const uint8_t iChannel = SerialNumbers::read_uint8();
		SerialNumbers::send_float(chDac.get_trigTime(iChannel));
		SerialNumbers::send_uint8(OK);  
	}
	else if (mode == SET_INPUT_PIN)
	{
		const uint8_t iChannel = SerialNumbers::read_uint8();
		define_input_pin(iChannel - 1); // -1 beacasue arduino number from 0, matlab from 1
		SerialNumbers::send_uint8(inputPin + 1);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_INPUT_PIN)
	{
		SerialNumbers::send_uint8(inputPin + 1);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == START_CASCADE)
	{
		SerialNumbers::send_uint8(OK);
		// here we now distinguish between the two modes
		if (nShots > 0)
		{
			start_ncascade();
		}
		else
		{
			start_scascade();
		}
	}
	else if (mode == STOP_CASCADE)
	{
		SerialNumbers::send_uint16(0);
		SerialNumbers::send_uint8(WARNING);
	}
	else if (mode == SET_TRIGTYPE)
	{
		inputTrigType = SerialNumbers::read_uint8();
		SerialNumbers::send_uint8(inputTrigType);
		SerialNumbers::send_uint8(OK);
	}
	else if (mode == GET_TRIGTYPE)
	{
		SerialNumbers::send_uint8(inputTrigType);
		SerialNumbers::send_uint8(OK);
	}
	else // this means invalid command
	{
		SerialNumbers::send_uint8(ERROR);
	}
	return;
}

void Cascader::define_input_pin(const uint8_t iChannel)
{
	pinMode(INPUT_PINS[iChannel], INPUT); //or INPUT_PULLUP
	inputPin = iChannel;
	return;
}

void Cascader::check_trigger()
{
	if (inputTrigType == TRIG_BOTH)
	{
		if(oldStatus ^ digitalReadFast(INPUT_PINS[inputPin])){	
			oldStatus = !oldStatus; // invert oldStatus
			cascade(); // start event loop
		 	iTrigger++;
		}
	}
	else if (inputTrigType == TRIG_RISING)
	{
		if(digitalReadFast(INPUT_PINS[inputPin])){	
			cascade(); // start event loop
		 	iTrigger++;

		 	while(digitalReadFast(INPUT_PINS[inputPin]))
			{
				// do nothing
			}
		}
	}
	else // if we are here it must be falling edge
	{
		if(!digitalReadFast(INPUT_PINS[inputPin])){	
			cascade(); // start event loop
		 	iTrigger++;

		 	while(!digitalReadFast(INPUT_PINS[inputPin]))
			{
				// do nothing
			}
		}		
	}
	return;
}

// starts the cascade until we are done with the specified number of iterationsd
void Cascader::start_ncascade()
{
	iTrigger = 0;
	digitalWriteFast(LED_STATUS, HIGH);
	oldStatus = digitalReadFast(INPUT_PINS[inputPin]);

	do{
		check_trigger();
	}while (iTrigger < nShots);

	digitalWriteFast(LED_STATUS, LOW);

	// here we do not send back the number of trigger events since they are predefined 
	SerialNumbers::send_uint8(OK);

	return;
}

// in this case we run the cascade until we receive a serial interupt
void Cascader::start_scascade()
{
	digitalWriteFast(LED_STATUS, HIGH);
	bool oldStatus = digitalReadFast(INPUT_PINS[inputPin]);
	uint32_t iTrigger = 0;
	do{
		check_trigger();
	}while (Serial.available() == 0);

	const uint8_t tempResponse = SerialNumbers::read_uint8();
	if (tempResponse == STOP_CASCADE) 
	{
		SerialNumbers::send_uint16(iTrigger);
		SerialNumbers::send_uint8(OK);
	}
	else
	{
		SerialNumbers::send_uint8(ERROR);
	}

	// once we are done with read the stop bit
	digitalWriteFast(LED_STATUS, LOW);
}


// starts a cascade following an incoming trigger event
void Cascader::cascade()
{
	uint32_t timeNS = 0;
	for(uint16_t iAverage = 0; iAverage < nAverages; iAverage++)
	{
		timeNS = 0; // set timer to 0
		do
		{
			WAIT_384_NS
			timeNS = timeNS + 700;
			chDac.update(timeNS); // each dac trigger contains another 12 NS sleep

			// update laser / pulser channels
			for (uint8_t iChannel = 0; iChannel < nChannels; iChannel++)
				chArray[iChannel].update(timeNS); // check if we need to set channel high or low
			
			}while (timeNS < endTime);

		// reset all flags in channel subclasses
		for(uint8_t iChannel = 0; iChannel < nChannels; iChannel++)
			chArray[iChannel].reset_flags();
		
		// get dac channel ready for next round
		chDac.reset_flags();
	}
	return;
}

