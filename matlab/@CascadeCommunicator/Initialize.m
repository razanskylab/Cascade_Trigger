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
	string = ['x'];

	% add rising and falling edge for each laser to string array
	for iLaser = 1:cc.nLasers
		string = [string, ...
			typecast(uint32(round(cc.timepoints(1, iLaser) * 1e3)), 'uint8'), ...
			typecast(uint32(round(cc.timepoints(3, iLaser) * 1e3)), 'uint8')];
	end

		% string = [tc.triggerType, ...
		% 	typecast(uint32(freq), 'uint8'), ...
		% 	typecast(uint32(tc.nShots), 'uint8')];

	% add trigger event for dac for each laser
	for iLaser = 1:cc.nLasers
		string = [string, ...
			typecast(uint32(floor(sortedDac(iLaser) * 1e3)), 'uint8')];
	end

	% add char indicating the number of averages
	string = [string, typecast(uint32(cc.nAverages), 'uint8')];
	% cc.nAverages
	% add char indicating delay between averages
	string = [string, typecast(uint32(ceil(cc.tMax) * 1e3), 'uint8')];
	% cc.tMax

	write(cc.S, char(string), 'uint8');
	% pause(0.3);
	response = char(read(cc.S, 5, 'string'));

	if ~strcmp(response(1:end-1), 'Init')
		error(['Could not initialize, response: ', response(1:end-1)]);
	else
		fprintf('done!\n')
	end

end