% File: Disconnect.m @ CascaderCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@ethz.ch
% Date: 23.11.2020

function Disconnect(cc)

<<<<<<< HEAD
	fprintf("[CascadeCommunicator] Discounneting device... ");
	tStart = tic();

	cc.S = [];

	fprintf("done after %.2f sec!\n", toc(tStart));
=======
	if (cc.isConnected)
		fprintf('[CascadeCommunicator] Discounneting device... ');
		cc.S = [];
		fprintf('done!\n');
		cc.isConnected = 0;
	else
		fprintf('[CascadeCommunicator] Device was not connected!\n');
	end
>>>>>>> e8f191b6d33bb43b4052eeeee064d94631371669

end