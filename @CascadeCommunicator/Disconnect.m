function Disconnect(cc)

	fprintf('[CascadeCommunicator] Discounneting device... ');

	fclose(cc.S);

	fprintf('done!\n');

end