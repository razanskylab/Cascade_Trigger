% File: Get_tDac.m @ CascadeCommunicator
% Author: Xiang Liu
% Mail: liuxian@student.ethz.ch
% Date: 04.07.2022

% Description: Pushes the trigger timepoints for the DAC over to our Teensy

function tDac = get.tDac(cc)
	write(cc.S, cc.GET_TDAC, "uint8");
	for iT = 1:cc.nLasers
		tDac(iT) = read(cc.S, 1, "uint8");
	cc.Handshake();
end