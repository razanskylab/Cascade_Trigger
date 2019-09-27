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
			char(round(cc.timepoints(1, iLaser))), ...
			char(round(cc.timepoints(3, iLaser)))];
	end

	% add trigger event for dac for each laser
	for iLaser = 1:cc.nLasers
		string = [string, char(floor(sortedDac(iLaser)))];
	end

	% add char indicating the number of averages
	string = [string, char(cc.nAverages)];
	% add char indicating delay between averages
	string = [string, char(ceil(cc.tAcquire))]

	fprintf(cc.S, string);
	% pause(0.3);
	response = fscanf(cc.S);

	if ~strcmp(response(1:end-1), 'Init')
		error(['Could not initialize, response: ', response(1:end-1)]);
	else
		fprintf('done!\n')
	end

end