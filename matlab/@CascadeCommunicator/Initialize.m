% File: Initialize.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofamnnu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: Sets up teensy

function Initialize(cc)

	cc.Clear_Serial_Input();
	fprintf('[CascadeCommunicator] Initializing... ');
	sortedDac = sort(cc.timepoints(4, :)); % sort trigger events of dac in rising order

	% first character needs to be an x to make sure that the teensy understands what we want
	write(cc.S, 'x', 'uint8');
	cc.Handshake();

	% add rising and falling edge for each laser to string array
	for iLaser = 1:cc.nLasers
		write(cc.S, uint32(round(cc.timepoints(1, iLaser) * 1e3)), 'uint32');
		write(cc.S, uint32(round(cc.timepoints(3, iLaser) * 1e3)), 'uint32');
	end

	% add trigger event for dac for each laser
	for iLaser = 1:cc.nLasers
		write(cc.S, uint32(floor(sortedDac(iLaser) * 1e3)), 'uint32');
	end

	% add char indicating the number of averages
	write(cc.S, uint32(cc.nAverages), 'uint32'); 

	% add char indicating delay between averages
	write(cc.S, uint32(ceil(cc.tMax) * 1e3), 'uint32'); 

	cc.Handshake();

	fprintf("done!\n");

end