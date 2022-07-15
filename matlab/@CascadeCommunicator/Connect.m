% File: Connect.m @ CascadeCommunicator
% Author: Urs Hofmann, Xiang Liu
% Mail: liuxian@student.eth.ch
% Date: 22.06.2022 
% Description: enables connection to cascade communicator

function Connect(cc)

	if (~cc.isConnected)
		tStart = tic();
		fprintf("[CascadeCommunicator] Opening connection... ");
		cc.S = serialport(cc.port, cc.baud_rate);
		tStop = toc(tStart); 
		fprintf("done after %.1f sec!\n", tStop);
	else
		fprintf("[CascadeCommunicator] Using existing connection.\n");
	end


end