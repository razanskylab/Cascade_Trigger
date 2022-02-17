% File: StartN.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 14.05.2020

function StartN(cc, varargin)

	if nargin == 1
		% default class values
	elseif nargin == 2
		cc.nShots = varargin{1};
	else
		error("Invalid number of arguments passed to function");
	end

	if ~cc.isConnected
		error("Board needs to be connected to use this function");
	end

	% set the correct trigger type
	if cc.trigType
		warning('[Cascader] setting trigger type to rising edges ONLY!\n');
	else
		write(cc.S, 'b', 'uint8'); % tell Teensy to set trigger type to BOTH rising and falling edges
		cc.Handshake();
	end

	write(cc.S, 'n', 'uint8');
	cc.Handshake();
	
	write(cc.S, uint32(cc.nShots), 'uint32');
	response = read(cc.S, 1, 'uint32'); % confirm # of trigger events received by Teensy
	if (response ~= cc.nShots)
		error('Teensy misunderstood something');
	end

	cc.Handshake(); % the last Teensy check before triggering

end