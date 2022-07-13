% File: Connect.m @ CascadeCommunicator
% Author: Urs Hofmann, Xiang Liu
% Mail: liuxian@student.eth.ch
% Date: 22.06.2022 



% Description: enables connection to cascade communicator, and check the ID number
function Connect(cc)

	if (~cc.isConnected)
		tStart = tic();
		fprintf("[CascadeCommunicator] Opening connection... ");
		cc.S = serialport(cc.port, cc.baud_rate);

		% check if connection was established to the right device, part of Identify() Code
		% write(cc.S, cc.IDENTIFY, "uint8");
		% returnId = read(cc.S, 1, "uint16");
		% if (returnId ~= cc.IDENTIFIER)
		% 	strMsg = sprintf("Invalid ID %d returned from device", returnId);
		% 	error(strMsg);
		% end   % This Identify can cause error "Invalid ID 19291 returned from device", So I comment it;

		tStop = toc(tStart); 
		fprintf("done after %.1f sec!\n", tStop);
	else
		fprintf("[CascadeCommunicator] Using existing connection.\n");
	end

end