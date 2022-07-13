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

	fprintf("[CascadeCommunicator] Starting cascader... ");
	write(cc.S, cc.START_CASCADE, "uint8");
	cc.Handshake();
	fprintf("done!\n");

end