% File: Start.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Starts the multiwavelength trigger on teensy

function Start(cc)

	write(cc.S, cc.START_CASCADE, "uint8");
	cc.Handshake();
	
end