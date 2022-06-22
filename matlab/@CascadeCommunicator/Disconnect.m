% File: Disconnect.m @ CascaderCommunicator
% Author: Urs Hofmann, Xiang Liu
% Mail: liuxian@student.eth.ch
% Date: 22.06.2022

function Disconnect(cc)

	if (cc.isConnected)
		fprintf('[CascadeCommunicator] Discounneting device... ');
		cc.S = [];
		fprintf('done!\n');
	else
		fprintf('[CascadeCommunicator] Device was not connected!\n');
	end

end