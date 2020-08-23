% File: Clear_Serial_Input.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 22.05.2020

function Clear_Serial_Input(cc)

	flush(cc.S);
	
end