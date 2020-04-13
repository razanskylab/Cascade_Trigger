#include "QuadReadout.h"

QuadReadout::QuadReadout(){
	pinMode(HCTL_RST_PIN, OUTPUT);
	pinMode(HCTL_OE_PIN, OUTPUT);
	pinMode(HCTL_SEL_PIN, OUTPUT);
	pinMode(HCTL_MANUAL_RESET_PIN, INPUT);
	digitalWriteFast(HCTL_RST_PIN, HIGH);
	digitalWriteFast(HCTL_OE_PIN, HIGH);
	digitalWriteFast(HCTL_SEL_PIN, HIGH);
}

QuadReadout::~QuadReadout(){
	// nothing here yet but an empty destructor
}

// reset counter chip values to zero
void QuadReadout::reset_hctl(){
	digitalWriteFast(HCTL_RST_PIN, LOW); // start read
 	WAIT_60_NS;
  	digitalWriteFast(HCTL_RST_PIN, HIGH);
  	return;
}

void QuadReadout::update_counter(){
	digitalWriteFast(HCTL_SEL_PIN, LOW); // select high bit
	digitalWriteFast(HCTL_OE_PIN, LOW); // start read

	WAIT_100_NS; // allow high bit to be stable
	((unsigned char *)&posCounter)[1] = GPIOD_PDIR & 0xFF; // read msb

	digitalWriteFast(HCTL_SEL_PIN, HIGH); // select low bit
	// get msb, write directly to counter, also turns uint to int...lots of magic here
	// we do this after changing pin, as data now needs time to settle...
	// ((uint8_t *)&counter)[1] = msb;

	WAIT_100_NS; // allow high bit to be stable
	((unsigned char *)&posCounter)[0] = GPIOD_PDIR & 0xFF; // read lsb
	// finish read out
	digitalWriteFast(HCTL_OE_PIN, HIGH);
	// digitalWriteFast(HCTL_SEL_PIN, HIGH);
	// might need to add delay here...

	// get lsb, write directly to counter, also turns uint to int...lots of magic here
	// ((uint8_t *)&counter)[0] = lsb;
	// if (posCounter == minusOne)
	//   Serial.println("OVERFLOW WARN!");
	return;
}

void QuadReadout::start(){

}

void QuadReadout:: startN(){

}

void QuadReadout::setXResolution(const double& _resolution){
	// Check if resolution is within desired range
	if (_resolution <= 0){
		resolution = 1 / ((double) encoderResolution); 
		// is defined as the minimum resolution
	}
	else if (_resolution >= 50000){
		resolution = 50000;
	}
	else{
		resolution = _resolution;
	}
	
	// convert resolution into counterLimit
	counterLimit = resolution * encoderResolution; 
	// --> maximum of 100,000 at 50000 resolution

}

void QuadReadout::setNTrigger(unsigned short& _nTrigger){
	nTrigger = _nTrigger;
}
