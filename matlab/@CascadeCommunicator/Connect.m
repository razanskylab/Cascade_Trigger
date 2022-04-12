% File: Connect.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 25.03.3022 

function Connect(cc)
	
	tStart = tic();
	fprintf("[CascadeCommunicator] Connecting to device... ");
	cc.S = serialport(cc.port, cc.BAUD_RATE);
	configureTerminator(cc.S, cc.TERMINATOR);
	fprintf("done after %.2f sec!\n", toc(tStart));
end