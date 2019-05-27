% File: Connect.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: enables connection to teensy

function Connect(cc)

	fprintf('[CascadeCommunicator] Connecting to device... ');

	% open serial connection
	fopen(cc.S);

	if strcmp(cc.S.Status, 'open');
		fprintf('done!\n');
	else
		error('Could not open connection!');
	end


end