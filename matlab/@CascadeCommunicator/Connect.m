% File: Connect.m @ CascadeCommunicator
% Mail: mail@hofmannu.org
% Date: 15.01.2022

% Description: enables connection to cascade communicator

function Connect(cc)

	if (~cc.isConnected)
		tStart = tic();
		fprintf("[CascadeCommunicator] Opening connection... ");
		cc.S = serialport(cc.port, cc.baud_rate);
		% configureTerminator(cc.S, 'CR');
		fprintf("done after %.1f sec!\n", toc(tStart));

		% check if connection was established to the right device
		write(cc.S, 'j', 'uint8');
		response = read(cc.S, 13, 'char'); 
		if isempty(response)
			error("empty response from cascader");
		end

		if strcmp(response, "TeensyCascade")
			cc.isConnected = 1;
		else
			cc.S = [];
			error("You are connected to the wrong device");
		end

		cc.Handshake();
	else
		fprintf("[CascadeCommunicator] Using existing connection.\n");
	end

end