% File: CascadeCommunicator.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: MATLAB class used to communicate with cascade teensy

classdef CascadeCommunicator < handle

	properties
		wavelengths(1, :) double {mustBePositive, mustBeFinite} = [532, 1064, 700];
		% order delay(1) is onda532, delay(2) is dye, delay(3) is onda1064 
		delay(1, 3) double = [0.2, 19, 0.2]; % delay between trigger and lasershot in micros
		triggerLength(1, 3) double = [2.1, 5, 2]; % for how long do we need to trigger the laser
		tAcquire double = 4.8276; % time for data acquisition in micros
		timepoints(4, 3) double; 
		nAverages(1, 1) uint32 = 1;
		% first dim is 1: risingEdge, 2: expected shot timepoint, 3: fallingEgdgestop trigger, 4: DAC trigger
		% second dim laser id, order is: Onda532, edge/dye, onda1064
		nShots(1, 1) uint16;
	end

	properties (SetAccess = private)
		port;
	end

	properties (SetAccess = private, Hidden)
		S; % serialport object
		nLasers(1, 1) = 3; % different from nWavelength since it is the hardware side
		% needs to be compatible with NLASERS in arduino code
	end

	properties (Dependent)
		nWavelengths(1, 1);
		tMax(1, 1) double; % overall length of trigger cascade in micros
	end

	methods

		% Constructor
		function cc = CascadeCommunicator()
			% check if file exists and if so load port from there
			if isfile(get_path('com_file'))
				load(get_path('com_file'), 'port_cascadecommunicator'); % load file
				cc.port = port_cascadecommunicator; % read com port
			else % otherwise determine com port
				cc.Find_Com_Port();
			end

			cc.S = serialport(cc.port, 115200);
			configureTerminator(cc.S, 'CR');

		end

		% Destructor
		function delete(h)
			h.Disconnect();
		end

		Find_Com_Port(cc);
		Connect(cc); % stablish connection to serial device
		Disconnect(cc); % close connection to serial device
		Initialize(cc); % initialize serial device
		Start(cc); % 
		SetN(cc); % define number of trigger events
		StartN(cc); % start N trigger events
		Stop(cc);
		Calculate(cc);
		tEarliest = Calculate_Channel_Times(cc, tEarliest, iLaser);
		Plot_Channel(cc, iLaser, laserColor);
		Clear_Serial_Input(tc);

		function nWavelengths = get.nWavelengths(cc)
			nWavelengths = length(cc.wavelengths);
		end

		% returns overall length of trigger cascade
		function tMax = get.tMax(cc)
			tMaxDac = cc.timepoints(4, :);
			% replace 255 in timepoints by 0 before we get max
			tMaxDac(tMaxDac == 255) = 0;
			tMax = max(tMaxDac(:)) + cc.tAcquire;
		end

	end

end