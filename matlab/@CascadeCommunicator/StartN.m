% File: StartN.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 14.05.2020

function StartN(cc)

	cc.Clear_Serial_Input();
	% string = ['n', typecast(uint16(cc.nShots), "uint8")];
	write(cc.S, 'n', "uint8");

	response = char(readline(cc.S));
	if ~strcmp(response, "r")
		error('Teensy did not give handshake for StartN command');
	end

end