% File: Start.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Starts the multiwavelength trigger on teensy

function Start(cc)

	cc.Clear_Serial_Input();

	write(cc.S, 's', "uint8");
	response = read(cc.S, 6, "string");

	% check response
	if ~strcmp(response(1:end-1), 'Start')
		error(['Could not start cascade trigger, response: ', response(1:end-1)]);
	end

end