% File: Disconnect.m @ CascaderCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@ethz.ch
% Date: 23.11.2020

function Disconnect(cc)

	if (cc.isConnected)
		fprintf('[CascadeCommunicator] Discounneting device... ');
		cc.S = [];
		fprintf('done!\n');
		cc.isConnected = 0;
	else
		fprintf('[CascadeCommunicator] Device was not connected!\n');
	end

end