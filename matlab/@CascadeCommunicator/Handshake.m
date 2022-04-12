% File: Handshake.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 25.03.2022

% Description: performs a handshake with the device

function Handshake(cc)

	returnVal = read(cc.S, 1, "uint8");
	if (returnVal == cc.OK)
		% nothing to do, handshake was nice
	elseif (returnVal == cc.WARNING)
		warning("Controller did throw warning");
	elseif (returnVal == cc.ERROR)
		error("Something went wronmg during execution, microcontroller returned error");
	elseif (isempty(returnVal))
		error("The microcontroller did not return anything (empty response)");
	else
		errMsg = sprintf("Invalid return from microcontroller during handshake: %d", ...
			returnVal);
		error(errMsg);
	end

end