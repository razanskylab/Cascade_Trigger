#include "Cascader.h"

volatile unsigned char time100NS = 0;

void increase_timer(){
	time100NS++;
}

// starts a cascade following an incoming trigger event
void Cascader::start_cascade(){

	for(uint8_t iAverage = 0; iAverage < nAverages; iAverage++){
		time100NS = 0; // set timer to 0

		myTimer.begin(increase_timer, 1);

		// start cascade
		do{
			chDac.update(time100NS);
			chArray[0].update(time100NS); // check if we need to set channel high or low
			chArray[1].update(time100NS); // check if we need to set channel high or low
			chArray[2].update(time100NS); // check if we need to set channel high or low
		}while (time100NS < endTime);

		myTimer.end();

		// reset all flags in channel subclasses
		for(uint8_t iChannel = 0; iChannel < nChannels; iChannel++){
			chArray[iChannel].reset_flags();
		}
		
		// get dac channel ready for next round
		chDac.reset_flags();
	}

}


// initialize cascader
void Cascader::init(
	const uint8_t timepoints[NCHANNELS * 2],
	uint8_t timepointsDac[NCHANNELS],
	const uint8_t _nAverages,
	const uint8_t _tAcquire){

	// push start and stop times to channel class
	for(uint8_t iChannel = 0; iChannel < NCHANNELS; iChannel++){
		chArray[iChannel].setOnTime(static_cast<uint8_t>(timepoints[iChannel * 2]));
		chArray[iChannel].setOffTime(static_cast<uint8_t>(timepoints[iChannel * 2 + 1]));
	}

	// push toggle time points 
	chDac.setTrigTimes(timepointsDac, nChannels);

	// find max value in array and use it as end time of procedure
	endTime = 0;
	for(uint8_t iTimepoint = 0; iTimepoint < NCHANNELS * 2; iTimepoint++){
		if (timepoints[iTimepoint] > endTime){
			endTime = static_cast<uint8_t>(timepoints[iTimepoint]);
		}
	}

	// check if dac timepoints are higher then channel trigger times
	for(uint8_t iTimepoint = 0; iTimepoint < NCHANNELS; iTimepoint++){
		// teimpoint = 255 means inactive channel so please ignore this
		if ((timepointsDac[iTimepoint] > endTime) & (timepointsDac[iTimepoint] < 255)){
			endTime = static_cast<uint8_t>(timepointsDac[iTimepoint]);
		}
	}

	// add 1 micros to end time to make sure that everything is done before we leave our loop
	endTime = endTime + _tAcquire;
	tAcquire = _tAcquire;
	nAverages = _nAverages;
}
