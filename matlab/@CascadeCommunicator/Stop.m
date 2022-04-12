% File: Stop.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.05.2019

% Description: Stops cascader and reads the number of received trigger events.

function Stop(cc)

	fprintf("[CascadeCommunicator] Stopping cascader... ");

	write(cc.S, cc.STOP_CASCADE, "uint8");
	nTrigger = read(cc.S, 1, "uint16");
	res = cc.Handshake();

	fprintf("done after %d trigger event(s)!\n", lastCascCount);

end