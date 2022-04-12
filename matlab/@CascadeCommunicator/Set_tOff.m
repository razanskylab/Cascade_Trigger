% File: Set_tOff.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 28.03.2022

% Description: defines the off time for a certain channel

function Set_tOff(cc, iChannel, tOff)

	iChannel = uint8(iChannel);
	tOff = single(tOff);

	% push all the information to our belovedP microcontroller
	write(cc.S, cc.SET_TOFF, "uint8");
	write(cc.S, uint8(iChannel), "uint8");
	write(cc.S, single(tOff), "single");

	responseChannel = read(cc.S, 1, "uint8");
	responseTOff = read(cc.S, 1, "single");
	cc.Handshake();

	if (responseChannel ~= iChannel)
		error("wrong channel ID returned while setting offtime");
	end

	if (responseTOff ~= tOff)
		error("off time was different from set value");
	end

end