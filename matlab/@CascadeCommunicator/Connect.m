% File: Connect.m @ CascadeCommunicator
% Author: Urs Hofmann, Xiang Liu
% Mail: liuxian@student.eth.ch
% Date: 22.06.2022 
% <<<<<HEAD
% function Connect(cc)
	
% 	tStart = tic();
% 	fprintf("[CascadeCommunicator] Connecting to device... ");
% 	cc.S = serialport(cc.port, cc.BAUD_RATE);
% 	configureTerminator(cc.S, cc.TERMINATOR);
% 	fprintf("done after %.2f sec!\n", toc(tStart));
% =======
% Mail: mail@hofmannu.org 
% Date: 15.01.2022

% Description: enables connection to cascade communicator

function Connect(cc)

	if (~cc.isConnected)
		tStart = tic();
		fprintf("[CascadeCommunicator] Opening connection... ");
		cc.S = serialport(cc.port, cc.baud_rate);

		% check if connection was established to the right device
		% write(cc.S, 'j', 'uint8');
		% response = read(cc.S, 13, 'char'); 
		% if isempty(response)
		% 	error("empty response from cascader");
		% end

		% if strcmp(response, "TeensyCascade")
		% 	cc.isConnected = 1;
		% else
		% 	cc.S = [];
		% 	error("You are connected to the wrong device");    %delete this
		% end

		%cc.Handshake();
		%cc.isConnected = 1;
		tStop = toc(tStart); 
		fprintf("done after %.1f sec!\n", tStop);
	else
		fprintf("[CascadeCommunicator] Using existing connection.\n");
	end


% >>>>>>> e8f191b6d33bb43b4052eeeee064d94631371669
end