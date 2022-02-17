% File: Start.m @ TeensyCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 28.04.2020

% Description: Starts the position based trigger scheme.
% Changelog:
% 		- include handshake if procedure done

function Start(cc)

	if ~cc.isConnected
		error("Cannot start procedure");
	end

	% set the correct trigger type
	if cc.trigType
		warning('[Cascader] setting trigger type to rising edges ONLY!\n');
	else
		write(cc.S, 'b', 'uint8'); % tell Teensy to set trigger type to BOTH rising and falling edges
		cc.Handshake();
	end

	fprintf("[CascadeCommunicator] Starting cascader... ");
	write(cc.S, 's', 'uint8');
	cc.Handshake();
	fprintf("done!\n");

end