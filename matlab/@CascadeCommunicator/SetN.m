% File: SetN.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 28.05.2020

function SetN(cc)

	cc.Clear_Serial_Input();
	string = ['d', typecast(uint16(cc.nShots), "uint8")];

	write(cc.S, string, "uint8");

	response = char(read(cc.S, 2, "string"));
	if ~strcmp(response(1), "r")
		error('Teensy did not give handshake for StartN command');
	end

end