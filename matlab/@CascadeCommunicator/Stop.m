% File: Stop.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.05.2019

% Description: Stops cascader and reads the number of received trigger events.

function nTrigger = Stop(cc)

	write(cc.S, cc.STOP_CASCADE, "uint8");
	nTrigger = read(cc.S, 1, "uint16");
	cc.Handshake();

end