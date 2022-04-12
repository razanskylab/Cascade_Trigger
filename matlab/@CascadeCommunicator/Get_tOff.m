% File: Get_tOff.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 28.03.2022

% Description: returns the off time for a specific channel

function tOff = Get_tOff(cc, iChannel)

	iChannel = uint8(iChannel);

	write(cc.S, cc.GET_TOFF, "uint8");
	write(cc.S, iChannel, "uint8");

	% conform correct ID first
	returnVal = read(cc.S, 1, "uint8");
	if (returnVal ~= iChannel)
		error("Wrong ID was returned from microcontroller");
	end

	tOff = read(cc.S, 1, "single");
	cc.Handshake();

end