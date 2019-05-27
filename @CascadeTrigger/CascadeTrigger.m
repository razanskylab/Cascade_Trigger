% non functional example class to be used as basis for new hardware interfacing
% class, as they alls should have similar structure and content

classdef CascadeTrigger < BaseHardwareClass
  properties
    triggerMask(1,8) uint16 {mustBeInteger,mustBeNonnegative} = [0,0,0,0,0,0,0,0];
      % defines which channels to trigger
    delays(1,8) uint16 {mustBeInteger,mustBeNonnegative} = [0,0,0,0,0,0,0,0];% [us]
      % defines delays for each channel
    durations(1,8) uint16 {mustBeInteger,mustBeNonnegative} = [0,0,0,0,0,0,0,0];% [us]
      % defines on time for each trigger

  end

  % depended properties are calculated from other properties
  properties (Dependent = true)
    bytesAvailable(1,1) {mustBeNumeric}; % [counts] current stage position, read from quad encoder
  end

  % things we don't want to accidently change but that still might be interesting
  properties (SetAccess = private, Transient = true)
    serialPtr = []; % pointer to serial port (we are using MEX Serial instead)
    isConnected = false;
  end

  % things we don't want to accidently change but that still might be interesting
  properties (Constant)
    % serial properties
    SERIAL_PORT = 'COM11';
    BAUD_RATE = 9600;

    DO_AUTO_CONNECT = true; % connect when object is initialized?

    %% Comands defined in teensy_lib.h
    DO_NOTHING = uint16(0);
    RECORD_CALIB_DATA = uint16(11);
    SEND_CURRENT_POS = uint16(12);
    SEND_CALIB_DATA = uint16(22);
    RESET_HCTL_COUNTER = uint16(33);
    RESET_TEENSY = uint16(44);
    ENABLE_POS_TRIGGER = uint16(55);
    DISABLE_POS_TRIGGER = uint16(56);
    CHECK_CONNECTION = uint16(98);
    DONE = uint16(99);
  end

  % same as constant but now showing up as property
  properties (Hidden=true)
  end


  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % constructor, desctructor, save obj
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods
    % constructor, called when class is created
    function CT = CascadeTrigger(doConnect)
      if nargin < 1
        doConnect = CT.DO_AUTO_CONNECT;
      end

      if doConnect && ~CT.isConnected
        CT.Connect;
      elseif ~CT.isConnected
        CT.VPrintF('[CT] Initialized but not connected yet.\n');
      end
    end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    function delete(CT)
      if ~isempty(CT.serialPtr) && CT.isConnected
        CT.Close();
      end
    end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % when saved, hand over only properties stored in saveObj
    function SaveObj = saveobj(CT)
      SaveObj = CascadeTrigger.empty; % see class def for info
    end
  end


  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods % short methods, which are not worth putting in a file

    function [] = Write_Command(CT,command)
      if ~isa(command,'uint16')
        error('Counter commands must be uint16!');
      end
      command = typecast(command, 'uint8'); % commands send as 2 byte
      CT.Write_Data(command);
    end

    function [success] = Check_Connection(CT)
      CT.PrintF('[CT] Checking teensy connection');
      CT.Write_Command(CT.CHECK_CONNECTION);
      success = CT.Wait_Done();
      if success
        CT.VPrintF('...looking good!\n');
      else
        CT.VPrintF('...teensy requires reset!\n');
      end
    end

    function [] = Write_16Bit(CT,data)
      CT.Write_Command(data); % same as command, but lets not confuse our users...
    end

  end

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods (Access = private)
  end

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods % set / get methods
    function [bytesAvailable] = get.bytesAvailable(CT)
      if CT.isConnected
        numBytesToRead = 0;
        [~ , bytesAvailable] = readPort(CT.serialPtr, numBytesToRead);
      else
        bytesAvailable = [];
      end
    end
  end % <<<<<<<< END SET?GET METHODS

end % <<<<<<<< END BASE CLASS
