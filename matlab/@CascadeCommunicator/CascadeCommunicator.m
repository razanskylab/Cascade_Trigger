% File: CascadeCommunicator.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: MATLAB class used to communicate with cascade teensy

classdef CascadeCommunicator < handle

	properties
		wavelengths(1, :) double {mustBeFinite} = [532, 1064, 700, 0];
		% a wavelength of 0 represents ultrasound pulse echo

		flagVerbose(1, 1) logical = 1; % enable / disable verbose output
		flagDisplay(1, 1) logical = 0; % enable / disable visual output
	end

	properties (SetAccess = private)
		port; % property used to store the serial port object

		triggerLength(1, 4) double = [2, 2, 2, 2]; % for how long do we need to trigger the laser
		delay(1, 4) double = [0, 16, 0, 0]; % delay between trigger and lasershot in micros
		% order delay:
		% 		1 -->   onda532
		% 		2 -->   dye
		%       3 -->   onda1064
		%       4 -->   ultrasound
		
		% rising edge of the trigger for channels, prev: timepoints(1, :)
		tOn(1, 4) single;

		% rising edge of the trigger for channels, prev: timepoints(3, :)
		tOff(1, 4) single;

		% timepoints when we trigger the dac, prev: timepoints(4, :)
		tDac(1, 4) single; 

		% timepoints when we expect the laser to actually shoot
		tFire(1, 4) single;

	end

	properties (SetAccess = private, Hidden)
		S; % serialport object
		nLasers(1, 1) = 4; % different from nWavelength since it is the hardware side
		% needs to be compatible with NLASERS in arduino code
		baud_rate(1, 1) = 115200;
	end

	properties (Dependent)
		nWavelengths(1, 1);  % how many wavelength totally
		tMax(1, 1) double; % overall length of trigger cascade in micros
		nShots(1, 1) uint32; % total number of shots (0 means without end)
		nAverages(1, 1) uint32; % number of averages per position/timepoint
		tAcquire(1, 1) single; % total data acquisition time in micros
		inputPin(1, 1) uint8; % "Value 1 is right for Pos board, connect to Teensy Pin16" SMA pin used as input [1 ... 4], 1 means in Arduino 0, because ArduinoPin +1
		trigType(1, 1);
		isConnected(1, 1) logical = 0;
	end

	properties(Constant, Hidden)
		% properties of serial connection
		TERMINATOR(1, :) char = 'CR';

		% dictionary
		IDENTIFY(1, 1) uint8 = 00;
		IDENTIFIER(1, 1) uint16 = 75; % unique device for AZbsoluteQuad teensy

		% DEFINE VARIABLES
		SET_AVERAGES(1, 1) uint8 = 11;
		SET_NSHOTS(1, 1) uint8 = 12;
		SET_TACQUIRE(1, 1) uint8 = 13;
		SET_TON(1, 1) uint8 = 14;
		SET_TOFF(1, 1) uint8 = 15;
		SET_TDAC(1, 1) uint8 = 16;
		SET_INPUT_PIN(1, 1) uint8 = 17;
		SET_TRIGTYPE(1, 1) uint8 = 18;

		% RETURN VARIABLES
		GET_AVERAGES(1, 1) uint8 = 21;
		GET_NSHOTS(1, 1) uint8 = 22;
		GET_TACQUIRE(1, 1) uint8 = 23;
		GET_TON(1, 1) uint8 = 24;
		GET_TOFF(1, 1) uint8 = 25;
		GET_TDAC(1, 1) uint8 = 26;
		GET_INPUT_PIN(1, 1) uint8 = 27;
		GET_TRIGTYPE(1, 1) uint8 = 28;

		% START
		START_CASCADE(1, 1) uint8 = 31;
		STOP_CASCADE(1, 1) uint8 = 32;

		TRIG_RISING(1, 1) uint8 = 81; % trigger on rising edge
		TRIG_FALLING(1, 1) uint8 = 82; % trigger on falling edge
		TRIG_BOTH(1, 1) uint8 = 83; % trigger on rising and falling edge

		% STATUS INDICATORS
		OK(1, 1) uint8 = 91;
		WARNING(1, 1) uint8 = 92;
		ERROR(1, 1) uint8 = 93;
		UNKNOWN_COMMAND(1, 1) uint8 = 94
	end

	methods


		function cc = CascadeCommunicator(varargin)

			if (nargin == 0)
				% check if file exists and if so load port from there	
				cc.port = get_com_port('Cascader'); % read com port
			elseif (nargin == 1)
				cc.port = varargin{1};
			else
				error("Invalid number of input arguments");
			end
			cc.Connect();
		end



		% Destructor
		function delete(h)
			h.Disconnect();
		end

		Connect(cc); % stablish connection to serial device
		Disconnect(cc); % close connection to serial device
		Identify(cc); % makes led blink and returns device id

		Set_tOff(cc, iChannel, tOff);
		tOff = Get_tOff(cc, iChannel);
		Set_tOn(cc, iChannel, tOn);
		tOn = Get_tOn(cc, iChannel);

		Set_tDac(cc, tDac);

		% not implemented / tested functions
		Find_Com_Port(cc);
		Initialize(cc); % initialize serial device
		Start(cc); % 
		SetN(cc); % define number of trigger events
		StartN(cc, varargin); % start N trigger events
		Stop(cc);
		StopN(cc);
		Set_Input_Pin(cc, varargin);
		Calculate(cc);
		tEarliest = Calculate_Channel_Times(cc, tEarliest, iLaser);
		Plot_Channel(cc, iLaser, laserColor);
		Clear_Serial_Input(tc);
		res = Handshake(tc); % performs handshake with device if everything worked nice
		Set_Trigger_Type(cc, trigType);

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

		function isConnected = get.isConnected(cc)
			if isempty(cc.S)
				isConnected = 0;
			else
				isConnected = 1;
			end
		end


		% set and get function for number of averages per trigger event
		function set.nAverages(cc, nAverages)
			write(cc.S, cc.SET_AVERAGES, "uint8");
			write(cc.S, uint32(nAverages), "uint32");
			returnVal = read(cc.S, 1, "uint32");
			if (returnVal ~= uint32(nAverages))
				error("Something went wrong while setting averages");
			end
			cc.Handshake();
		end

		function nAverages = get.nAverages(cc)
			write(cc.S, cc.GET_AVERAGES, "uint8");
			nAverages = read(cc.S, 1, "uint32");
			cc.Handshake();
		end

		% define the number of shots we take
		function set.nShots(cc, nShots)
			write(cc.S, cc.SET_NSHOTS, "uint8");
			write(cc.S, uint32(nShots), "uint32");
			returnVal = read(cc.S, 1, "uint32");
			if (returnVal ~= uint32(nShots))
				error("Something went wrong while setting shots");
			end
			cc.Handshake();
		end

		function nShots = get.nShots(cc)
			write(cc.S, cc.GET_NSHOTS, "uint8");
			nShots = read(cc.S, 1, "uint32");
			cc.Handshake();
		end

		% define the total data acquisition time tAcquire in micros
		function tAcquire = get.tAcquire(cc)
			write(cc.S, cc.GET_TACQUIRE, "uint8");
			tAcquire = read(cc.S, 1, "single");
			cc.Handshake();
		end

		function set.tAcquire(cc, tAcquire)
			write(cc.S, cc.SET_TACQUIRE, "uint8");
			write(cc.S, single(tAcquire), "single");
			returnVal = read(cc.S, 1, "single");
			if (returnVal ~= single(tAcquire))
				error("Something went wrong while setting acquisition time");
			end
			cc.Handshake(); 
		end

		% definintion of the input pin
		function inputPin = get.inputPin(cc)
			write(cc.S, cc.GET_INPUT_PIN, "uint8");
			inputPin = read(cc.S, 1, "uint8");
			cc.Handshake();
		end

		function set.inputPin(cc, inputPin)
			write(cc.S, cc.SET_INPUT_PIN, "uint8");
			write(cc.S, uint8(inputPin), "uint8");
			returnVal = read(cc.S, 1, "uint8");
			if (returnVal ~= uint8(inputPin))
				error("Microcontroller returned wrong input pin");
			end
			cc.Handshake();
		end

		function set.trigType(cc, trigType)
			% lookup key for currently selected trigger type
			if strcmp(trigType, "both")
				trigTypeKey = cc.TRIG_BOTH;
			elseif strcmp(trigType, "falling")
				trigTypeKey = cc.TRIG_FALLING;
			elseif strcmp(trigType, "rising")
				trigTypeKey = cc.TRIG_RISING;
			else
				error("Trigger type can be rising, falling, or both");
			end

			write(cc.S, cc.SET_TRIGTYPE, "uint8");
			write(cc.S, trigTypeKey, "uint8");
			response = read(cc.S, 1, "uint8");
			if (response ~= trigTypeKey)
				errMsg = sprintf("Teensy confirmed wrong trigger type: %d instead of %d", ...
					response, trigTypeKey);
				error(errMsg);
			end
			cc.Handshake();

		end

		function trigType = get.trigType(cc)
			write(cc.S, cc.GET_TRIGTYPE, "uint8");
			response = read(cc.S, 1, "uint8");

			if (response == cc.TRIG_FALLING)
				trigType = "falling";
			elseif (response == cc.TRIG_RISING)
				trigType = "rising";
			elseif (response == cc.TRIG_BOTH)
				trigType = "both";
			else
			 	trigType = "invalid";
			 	error("Trigger type returned from board is not valid");
			end

			cc.Handshake();
		end



	end

end