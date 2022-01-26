% File: Start.m @ TeensyCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 28.04.2020

% Description: Starts the position based trigger scheme.
% Changelog:
% 		- include handshake if procedure done

function Start(tc)

	if ~tc.isConnected
		error("Cannot start procedure");
	end

	fprintf("[CascadeCommunicator] Starting cascader... ");
	write(tc.S, 's', 'uint8');
	tc.Handshake();
	fprintf("done!\n");


end