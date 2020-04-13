#include "WaitNS.h"
#include "PinMapping.h"
#include "Arduino.h"

#ifndef QUADREADOUT_H
#define QUADREADOUT_H

class QuadReadout{
public:
	QuadReadout(); // constructor
	~QuadReadout(); // destructor
	
	void reset_hctl(); // reset counter chip values to zero
	void update_counter();
	void start();
	void startN();
	void setXResolution(const double& _resolution);
	void setNTrigger(unsigned short& _nTrigger);
private:
	uint16_t posCounter = 0; // stores the 16 bit version of out readout
	char encoderResolution = 2; // [1/microm]
	double resolution = 100; // step size of scan [µm]
	unsigned short nTrigger = 500; // number of trigger events per b scan
	short counterLimit; // step size in counter events of quad decoder board
};

#endif