% File: CascadeCommunicator.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: MATLAB class used to communicate with cascade teensy

classdef CascadeCommunicator < handle

	properties
		wavelengths(1, :) double {mustBeFinite} = [532, 1064, 700, 0];
		% a wavelength of 0 represents ultrasound pulse echo
		% order delay:
		% 		1 --> onda532
		% 		2 --> dye
		%     3 -->  onda1064

		delay(1, 4) double = [0, 16, 0, 0]; % delay between trigger and lasershot in micros
		triggerLength(1, 4) double = [2, 2, 2, 2]; % for how long do we need to trigger the laser
		tAcquire(1, 1) double = 5; % default data acquisition time [micros]
		timepoints(4, 4) double;
		% first dim is 1: risingEdge, 2: expected shot timepoint, 3: fallingEgdgestop trigger, 4: DAC trigger
		% second dim laser id, order is: Onda532, edge/dye, onda1064, us pulser
		nAverages(1, 1) uint32 = 1;
		nShots(1, 1) uint16;
	end

	properties (SetAccess = private)
		port;
	end

	properties (SetAccess = private, Hidden)
		S; % serialport object
		nLasers(1, 1) = 4; % different from nWavelength since it is the hardware side
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
			cc.port = get_com_port('Cascader'); % read com port
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
			tMaxDac(tMaxDac == intmax('uint32')) = 0;
			tMax = max(tMaxDac(:));

			if (tMax < cc.tAcquire)
				tMax = cc.tAcquire;
			end
		end

	end

end