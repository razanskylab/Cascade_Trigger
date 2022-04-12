% File: Identify.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 25.03.2022

% Description: identifies the currently connected microcontroller as 
% the cascade communicator

function Identify(cc)

	if (~cc.isConnected)
		error("Cannot perform this operation without conenction");
	end

	tStart = tic();
	fprintf("[CascadeCommunicator] Identifying device... ");

	write(cc.S, cc.ID, "uint8");
	returnId = read(cc.S, 1, "uint16");
	if (returnId ~= cc.IDENTIFIER)
		strMsg = sprintf("Invalid ID %d returned from device", returnId);
		error(strMsg);
	end

	cc.Handshake();
	fprintf("done after %.2f sec!\n", toc(tStart));

end