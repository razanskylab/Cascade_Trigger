% File: Set_tOn.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 28.03.2022

% Description: defines the on time for a certain channel

function Set_tOn(cc, iChannel, tOn)
	
	iChannel = uint8(iChannel);
	tOn = single(tOn);

	% push all the information to our beloved microcontroller
	write(cc.S, cc.SET_TON, "uint8");
	write(cc.S, uint8(iChannel), "uint8");
	write(cc.S, single(tOn), "single");

	responseChannel = read(cc.S, 1, "uint8");
	responseTOn = read(cc.S, 1, "single");
	cc.Handshake();

	if (responseChannel ~= iChannel)
		error("wrong channel ID returned while setting ontime");
	end

	if (responseTOn ~= tOn)
		error("off time was different from set value");
	end
end