% File: Initialize.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: Initializes hardware.

function Calculate(cc)

	C = Colors();

	% if a 0 stays in order this laser is unused, reset all lasers to unused first
	cc.timepoints = 0;

	% set all lasers to unused
	flagOnda532 = 0;
	flagDye = 0;
	flagOnda1064 = 0;
	flagUs = 0;

	tEarliest = 0; % in the beginning we could start the laser whenever we want

	figure('Name', 'CascaderPlot', 'NumberTitle', 'Off');

	for iWavelength = 1:cc.nWavelengths
		% if we are working with onda532
		if (cc.wavelengths(iWavelength) == 532)
			% check if laser was already defined as used
			if flagOnda532
				error('Laser already in use');
			else
				% fprintf("Onda532 will be used\n");	
			end
			
			% theoretical value where we could position rising edge
			tEarliest = cc.Calculate_Channel_Times(tEarliest, 1);
			cc.Plot_Channel(1, wavelength2color(cc.wavelengths(iWavelength)));
			title('Onda532');

			flagOnda532 = 1;
		% if we are working with edge/dye
		elseif ((cc.wavelengths(iWavelength) > 550) && (cc.wavelengths(iWavelength) < 900))
			% check if laser is already in use
			if flagDye
				error('Laser already in use');
			else
				% fprintf("Dye will be used\n");
			end

			% theoretical value where we could position rising edge
			tEarliest = cc.Calculate_Channel_Times(tEarliest, 2);
			cc.Plot_Channel(2, wavelength2color(cc.wavelengths(iWavelength)));
			title('Dye');
			
			flagDye = 1;
		elseif (cc.wavelengths(iWavelength) == 1064)
			% check if laser was already defined as used
			if flagOnda1064
				error('Laser already in use');
			else
				% fprintf("Onda1064 will be used\n");
			end

			% theoretical value where we could position rising edge
			tEarliest = cc.Calculate_Channel_Times(tEarliest, 3);
			cc.Plot_Channel(3, C.DarkRed);
			title('Onda1064');

			flagOnda1064 = 1;
		elseif (cc.wavelengths(iWavelength) == 0)
			if flagUs 
				error('Ultrasound already in use');
			else

			end
			tEarliest = cc.Calculate_Channel_Times(tEarliest, 4);
			cc.Plot_Channel(4, C.DarkBlue);
			title('Ultrasound');
			flagUs = 1;
		
		else
			error('No laser matches this wavelength range.');
		end
	end

	tMax = max(cc.timepoints(:));

	if (tMax < cc.tAcquire)
		tMax = cc.tAcquire;
	end

	for iPlot = 1:cc.nLasers
		subplot(cc.nLasers, 1, iPlot)
		xlim([0, tMax]);
	end

	% if laser is not active we don't want the corresponding dac event so we just set it 
	% to 255
	if (flagOnda532 == 0)
		cc.timepoints(4, 1) = intmax('uint32');
	end

	if (flagDye == 0)
		cc.timepoints(4, 2) = intmax('uint32');
	end
	
	if (flagOnda1064 == 0)
		cc.timepoints(4, 3) = intmax('uint32');
	end

	if (flagUs == 0)
		cc.timepoints(4, 4) = intmax('uint32');
	end

end