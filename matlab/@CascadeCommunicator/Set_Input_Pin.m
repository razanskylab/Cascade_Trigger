% File: Set_Input_Pin.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: mail@hofmannu.org
% Date: 20.01.2022

% Description: defines the cascader input pin

function Set_Input_Pin(cc, varargin)

	fprintf("[CascadeCommunicator] Defining cascader input pin... ");
	if nargin == 1
		% do nothing
	elseif nargin == 2
		cc.inputPin = varargin{1}; %set function
	else
		error("Invalid number of arguments passed to function");
	end
	fprintf("done!\n");


end