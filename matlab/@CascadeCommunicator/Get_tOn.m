% File: Get_tOn.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 28.03.2022

% Description: returns the on time for a specific channel

function tOn = Get_tOn(cc, iChannel)

	iChannel = uint8(iChannel);

	write(cc.S, cc.GET_TON, "uint8");
	write(cc.S, iChannel, "uint8");

	% conform correct ID first
	returnVal = read(cc.S, 1, "uint8");
	if (returnVal ~= iChannel)
		error("Wrong ID was returned from microcontroller");
	end

	tOn = read(cc.S, 1, "single");
	cc.Handshake();
	
end