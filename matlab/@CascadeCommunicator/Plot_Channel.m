% Function: Plot_Channel.m @ CascadeCommunicator
% Author: Urs Hofmann
% Mail: hofmannu@biomed.ee.ethz.ch
% Date: 25.04.2019

% Description: Creates simple plot of laser trigger pattern

function Plot_Channel(cc, iLaser, laserColor)
	subplot(cc.nLasers, 1, iLaser)
	hold on
	plot(...
		[0, cc.timepoints(1, iLaser), cc.timepoints(1, iLaser), cc.timepoints(3, iLaser), cc.timepoints(3, iLaser)], ...
		[0, 0, 1, 1, 0], ...
		':',  'Color', laserColor);
	plot([cc.timepoints(2, iLaser), cc.timepoints(2, iLaser)], [0, 1], 'Color', laserColor);
	fill(...
		[cc.timepoints(4, iLaser), cc.timepoints(4, iLaser), cc.timepoints(4, iLaser) + cc.tAcquire, cc.timepoints(4, iLaser) + cc.tAcquire], ...
		[0, 1, 1, 0], ...
		laserColor, 'EdgeColor', 'none', 'FaceAlpha', 0.25);
	hold off
end