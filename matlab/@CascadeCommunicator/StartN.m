% File: StartN.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 14.05.2020

function StartN(cc)

	cc.Clear_Serial_Input();
	string = ['n', typecast(uint32(cc.nShots), "uint8")];
	write(cc.S, char(string), 'uint8');

	response = char(readline(cc.S));
	idealResponse = ['r', num2str(cc.nShots)];
	if ~strcmp(response, idealResponse)
		error('Teensy did not give handshake for StartN command');
	end

end