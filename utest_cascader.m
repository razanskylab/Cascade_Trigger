clear all; close all;

C = CascadeCommunicator("COM16");
C.Disconnect();

for i = 1:10
	C.Connect();
	C.Disconnect();
end
C.Connect();

C.Identify(); 

% defining the number of averages our device needs to tolerate
fprintf("Setting number of averages... ");
nAv = uint32(1:10:100);
for (iAv = 1:length(nAv))
	C.nAverages = nAv(iAv);
	if (nAv(iAv) ~= C.nAverages)
		error("Something went wrong during our precious get function for averages");
	end
end
fprintf("done!\n");
C.nAverages = 10;

fprintf("Setting number of shots... ");
nShots = uint32(0:10:100);
for (iShot = 1:length(nShots))
	C.nShots = nShots(iShot);
	returnVal = C.nShots;
	if (nShots(iShot) ~= returnVal)
		errMsg = sprintf("get function gives bad result %d instead of %d for nShots", ...
			returnVal, nShots(iShot));
		error(errMsg);
	end
end
fprintf("done!\n");

% try to define different acquisition times
fprintf("Settings acquisition times... ");
tAcquire = single(0.0:0.2:6.0);
for iAc = 1:length(tAcquire)
	C.tAcquire = tAcquire(iAc);
	returnVal = C.tAcquire;
	if (tAcquire(iAc) ~= returnVal)
		errMsg = sprintf("get function gives bad result %f instead of %f for tAcuqire", ...
			returnVal, tAcquire(iAc));
		error(errMsg);
	end
end
fprintf("done!\n");

% define timepoints
fprintf("Setting laser timepoints... ");
timepoints = single(rand(2, 4)); 
for iLaser = 0:1:3
	C.Set_tOn(iLaser, timepoints(1, iLaser + 1));
	if C.Get_tOn(iLaser) ~= timepoints(1, iLaser + 1)
		error("incorrect on time returned");
	end 

	C.Set_tOff(iLaser, timepoints(2, iLaser + 1));
	if C.Get_tOff(iLaser) ~= timepoints(2, iLaser + 1)
		error("incorrect off time returned");
	end
end
fprintf("done!\n");

% set dac timepoints
fprintf("Setting DAC timepoints... ");
for iTest = 1:10
	C.Set_tDac([0.1 * iTest, 0.2 * iTest, 0.3 * iTest, 0.4 * iTest]);
end
fprintf("done!\n");

% full initialization including all four channels and DAC

C.wavelengths = [532, 1064, 700];
fprintf("Starting calculation... ");
C.Calculate();
fprintf("done!\n");
C.Initialize();

fprintf("Defining different input pins... ");
for iPin = 1:4
	C.inputPin = iPin;
	if (C.inputPin ~= iPin)
		error("Could not define input pin");
	end
end
fprintf("done!\n");
C.inputPin = 1;

C.trigType = "falling";
C.trigType
C.trigType = "rising";
C.trigType
C.trigType = "both";
C.trigType

C.nShots = 0;
C.Start();
C.Stop();



% this line should throw a warning
C.Stop();
C
%close all;
%clear all; 

%C.Set_tDac([1, 10, 100, 300]);