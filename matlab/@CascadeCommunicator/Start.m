% File: Start.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Starts the multiwavelength trigger on teensy

function Start(cc)

	fprintf("[CascadeCommunicator] Starting cascader... ");
	cc.Clear_Serial_Input();

	write(cc.S, 's', "uint8");
	response = read(cc.S, 1, "string");

	% check response
	if response(1) ~= 'r'
		error(['Could not start cascade trigger, response: ', response(1:end-1)]);
	else
		fprintf("done!\n");
	end
end