% File: Handshake.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 18.01.2022

% Description: Performs a handshake with our beloved cascader

function res = Handshake(cc)

	received = read(cc.S, 1, 'char');
	if isempty(received)
		error("Cascader did not send anything back");
	end

	if (received == 'r')
		% do nothing since handshake was successful
		res = 1;
	elseif (received == 'w')
		warning("Teensy returned a warning during handshake");
		res = 0;
	elseif (received == 'e')
		error("Teensy returned an error during handsake");
	else
		error("Teensy returned an unknown code");
	end

end