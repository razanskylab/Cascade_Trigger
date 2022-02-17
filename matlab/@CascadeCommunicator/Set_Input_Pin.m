% File: Set_Input_Pin.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 20.01.2022

% Description: defines the cascader input pin

function Set_Input_Pin(cc, varargin)

	if nargin == 1
		% do nothing
	elseif nargin == 2
		cc.inputPin = varargin{1};
	else
		error("Invalid number of arguments passed to function");
	end

	fprintf("[CascadeCommunicator] Defining cascader input pin... ");

	write(cc.S, 'p', "uint8");
	write(cc.S, uint8(cc.inputPin), "uint8");
	response = read(cc.S, 1, 'uint8');
	if (response ~= cc.inputPin)
		error("Something went wrong while defining the inputPin");
	end
	cc.Handshake();

	fprintf("done!\n");


end