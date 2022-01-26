% File: Stop.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.05.2019

% Description: Stops cascader and reads the number of received trigger events.

function Stop(cc)

	fprintf("[CascadeCommunicator] Stopping cascader... ");

	write(cc.S, 'o', "uint8");
	res = cc.Handshake();

	if res
		lastCascCount = read(cc.S, 1, 'uint32');
		if isfinite(lastCascCount)
			cc.lastCascCount = lastCascCount;
		end
		cc.Handshake();

		fprintf("done after %d trigger event(s)!\n", lastCascCount);
	else
		fprintf("was not running!\n");
	end
end