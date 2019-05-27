% File: Find_Com_Port.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: Automatically determines com port of cascade teensy

function Find_Com_Port(cc)

		% get list containing all com ports
	portArray = seriallist();

	nDevices = length(portArray);


	flagFound = 0; % indicates if com port was already found

	for iDevice = 1:nDevices
		
		% only perform this if we did not find teensy yet
		if flagFound == 0
			
			try
				obj = serial(portArray(iDevice));
				obj.BaudRate = 115200;
				obj.Timeout = 1;
				fopen(obj);
				fprintf(obj, 'i'); % request id
		    pause(0.1);
				response = fscanf(obj);
				if strcmp(response, 'TeensyCascade')
					fprintf('[TeensyCascade] Found TeensyCascade on port %s\n', portArray(iDevice));
					cc.port = portArray(iDevice);
					flagFound = 1;
				end

				fclose(obj);
			catch
				% do nothing
			end

		end

	end

	if flagFound == 0
		error('Could not find position based trigger anywhere.');
	else
		port_cascadecommunicator = cc.port % need to do this because we cannot save class properties directly
		if isfile(get_path('com_file')) % if file exists do not overwrite but append
			save(get_path('com_file'), 'port_cascadecommunicator', '-append');
		else
			save(get_path('com_file'), 'port_cascadecommunicator');
		end
	end

end