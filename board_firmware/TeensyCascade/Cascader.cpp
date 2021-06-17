#include "Cascader.h"



// starts a cascade following an incoming trigger event
void Cascader::start_cascade()
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


// initialize cascader, all timepoints passed in ns
void Cascader::init(
	const uint32_t* timepoints,
	const uint32_t* timepointsDac,
	const uint32_t _nAverages,
	const uint32_t _tAcquire){

	// push start and stop times to channel class
	for(uint8_t iChannel = 0; iChannel < NCHANNELS; iChannel++)
	{
		chArray[iChannel].setOnTime(timepoints[iChannel * 2]);
		chArray[iChannel].setOffTime(timepoints[iChannel * 2 + 1]);
	}

	
	chDac.setTrigTimes(timepointsDac, nChannels); // push toggle time points 

	// find max value in array and use it as end time of procedure
	endTime = 0;
	for(uint8_t iTimepoint = 0; iTimepoint < NCHANNELS * 2; iTimepoint++)
	{
		if (timepoints[iTimepoint] > endTime)
			endTime = static_cast<uint32_t>(timepoints[iTimepoint]);
	}

	// check if dac timepoints are higher then channel trigger times
	for(uint8_t iTimepoint = 0; iTimepoint < NCHANNELS; iTimepoint++){
		// timepoint == 4294967295 means inactive channel so please ignore this
		if ((timepointsDac[iTimepoint] > endTime) && (timepointsDac[iTimepoint] < 4294967295))
			endTime = static_cast<uint32_t>(timepointsDac[iTimepoint]);
	}

	// add some offset to end time to make sure that everything is done before we leave our loop
	endTime = endTime + _tAcquire;
	tAcquire = _tAcquire;
	nAverages = _nAverages;

	return;
}
