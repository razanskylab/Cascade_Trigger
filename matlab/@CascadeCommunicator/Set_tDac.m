% File: Set_tDac.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 28.03.2022

% Description: Pushes the trigger timepoints for the DAC over to our Teensy

function Set_tDac(cc, tDac)

	tDac = single(tDac); % make sure the array is a single

	nT = length(tDac);
	if (nT ~= cc.nLasers)
		error("Number of timepoints must match number of lasers");
	end

	write(cc.S, cc.SET_TDAC, "uint8");  
	for iT = 1:cc.nLasers
		write(cc.S, tDac(iT), 'single');
		response = read(cc.S, 1, "single");
		if (response ~= tDac(iT))
			errMsg = sprintf("Something went wrong while setting timpoint for laser %d", ...
				iT);
			error(errMsg);
		end
	end
	cc.Handshake();

end