% File: Initialize.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofamnnu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: Sets up teensy

function Initialize(cc)

	fprintf('[CascadeCommunicator] Initializing... ');
	tStart = tic();
	
	% sort trigger events of dac in rising order
	sortedDac = sort(cc.tDac); 
	cc.Set_tDac(sortedDac);

	% add rising and falling edge for each laser to string array
	for iLaser = 1:cc.nLasers
		cc.Set_tOn(iLaser - 1, cc.tOn(1, iLaser) * 1e3);
		cc.Set_tOff(iLaser - 1, cc.tOn(1, iLaser) * 1e3);
	end

	tStop = toc(tStart);
	fprintf("done after %.2f sec!\n", tStop);	

end