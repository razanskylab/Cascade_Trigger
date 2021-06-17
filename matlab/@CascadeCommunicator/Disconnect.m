% File: Disconnect.m @ CascaderCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@ethz.ch
% Date: 23.11.2020

function Disconnect(cc)

	fprintf('[CascadeCommunicator] Discounneting device... ');

	cc.S = [];

	fprintf('done!\n');

end