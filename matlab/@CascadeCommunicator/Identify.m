% File: Identify.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 18.01.2022

% Description: Identifies our CascadeCommunicator by blinking the LED

function Identify(cc)

	if (cc.isConnected)
		fprintf("[CascadeCommunicator] Identifying device... ");
		write(cc.S, 'i', 'uint8');
		response = read(cc.S, 13, 'char');
		if isempty(response)
			error("empty response from cascader");
		end

		if ~strcmp(response, "TeensyCascade")
			error("You are connected to the wrong device");
		end
		cc.Handshake();

		fprintf("done!\n");

	else
		error("You need to connect the CascadeCommunicator first");
	end

end