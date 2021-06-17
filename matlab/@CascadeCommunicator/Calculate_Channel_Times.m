% File: Calculate_Channel_Times.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: Calculates the optimal trigger times depending on wanted timepoint.

function tEarliest = Calculate_Channel_Times(cc, tEarliest, iLaser)

	% calculate earliest possible position for rising edge
	cc.timepoints(1, iLaser) = tEarliest - cc.delay(iLaser);
	
	% check if this position is below 0 which is not feasible
	if (cc.timepoints(1, iLaser) < 0)
		cc.timepoints(1, iLaser) = 0
	end

	% calculate timepoint where laser theoretically fires
	cc.timepoints(2, iLaser) = cc.timepoints(1, iLaser) + cc.delay(iLaser);

	% start DAC just below the theoretically calculated laser start
	cc.timepoints(4, iLaser) = cc.timepoints(2, iLaser) - 250e-3;

	% calculate position in time when we need falling edge
	cc.timepoints(3, iLaser) = cc.timepoints(1, iLaser) + cc.triggerLength(iLaser);

	tEarliest = cc.timepoints(2, iLaser) + cc.tAcquire + 1;

end