% File: Stop.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.05.2019

% Description: Stops cascader and reads the number of received trigger events.

function Stop(cc)

	cc.Clear_Serial_Input();
	write(cc.S, 'o', "uint8");

	% trigEvents = str2double(readline(cc.S));

	%{
if (trigEvents ~= cc.nShots)
		txtMsg = [num2str(trigEvents), ' trigger events only (missing ', ...
		num2str(double(cc.nShots) - trigEvents), ')'];
		warning(txtMsg);
	end
%}


end