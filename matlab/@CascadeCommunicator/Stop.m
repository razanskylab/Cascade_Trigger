% File: Stop.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.05.2019

% Description: Stops cascader and reads the number of received trigger events.

function Stop(cc)


	fprintf("[Cascader] Stopping cascader...");

	cc.Clear_Serial_Input();
	write(cc.S, 'o', "uint8");

	lastCascCount = str2double(readline(cc.S));
	if isfinite(lastCascCount)
		cc.lastCascCount = lastCascCount;
		fprintf("triggered %d times!\n", cc.lastCascCount);
	end

	fprintf("done!\n");

end