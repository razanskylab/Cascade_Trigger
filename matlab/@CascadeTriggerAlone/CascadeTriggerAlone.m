% non functional example class to be used as basis for new hardware interfacing
% class, as they alls should have similar structure and content

classdef CascadeTriggerAlone < BaseHardwareClass
  properties
    prf(1,1) {mustBeNumeric,mustBeNonnegative,mustBeFinite} = 100;
    triggerMask(1,8) uint16 {mustBeInteger,mustBeNonnegative} = [0,0,0,0,0,0,0,0];
      % defines which channels to trigger
    mode(1,:) char = 'us'; % set function ensures only valid modes are used!
  end

  % depended properties are calculated from other properties
  properties (Dependent = true)
    bytesAvailable(1,1) {mustBeNumeric}; % [counts] current stage position, read from quad encoder
    samplingPeriod(1,1) uint16 {mustBeInteger,mustBeNonnegative};
    slowSampling(1,1); % sets samplingPeriod in us or ms
  end

  % things we don't want to accidently change but that still might be interesting
  properties (SetAccess = private, Transient = true)
    serialPtr = []; % pointer to serial port (we are using MEX Serial instead)
    isConnected = false;
  end

  properties (SetAccess = private, Transient = true)
    lastTrigCount(1,1) {mustBeNumeric,mustBeNonnegative,mustBeFinite};
  end

  % things we don't want to accidently change but that still might be interesting
  properties (Constant)
    % serial properties
    SERIAL_PORT = 'COM13';
    BAUD_RATE = 9600;

    DO_AUTO_CONNECT = true; % connect when object is initialized?
    MAX_BYTE_PER_READ = 4096; % we can read this many bytes over serial at once

    %% Comands defined in teensy_lib.h

    % define commands %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    DO_NOTHING = uint16(00);
    SET_TRIGGER_CH = uint16(11);
    DO_TRIGGER = uint16(22);
    STOP_TRIGGER = uint16(23);
    ENABLE_SCOPE_MODE = uint16(66);
    DISABLE_SCOPE = uint16(67);
    CHECK_CONNECTION = uint16(88);
    DONE = uint16(99);

    % define trigger port bits %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    DAQ_TRIG = uint16(8);
    US_TRIG = uint16(7);
    ONDA_TRIG = uint16(5);
    EDGE_TRIG = uint16(4);
    DAQ_LED_PIN = uint16(3);
    ALL_TRIG = uint16(0);
  end

  % same as constant but now showing up as property
  properties (Hidden=true)
  end


  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % constructor, desctructor, save obj
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods
    % constructor, called when class is created
    function CT = CascadeTriggerAlone(doConnect)
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

    function [] = Enable_Trigger(CT)
      CT.Setup_Trigger();

      if CT.Wait_Done()
        CT.Done();
        tic;
        CT.VPrintF('[CT] Enabling trigger board...');
        CT.Write_Command(CT.DO_TRIGGER);
        CT.Done();
      else
        CT.Verbose_Warn('[CT] Trigger enable failed!\n');
      end
    end

    function [] = Disable_Trigger(CT)
      tic;
      CT.VPrintF('[CT] Disabling trigger board...');
      CT.Write_Command(CT.STOP_TRIGGER);
      CT.Done();
    end

    function [] = Update_Trigger(CT)
      CT.Disable_Trigger();
      CT.Enable_Trigger();
    end

    % --------------------------------------------------------------------------
    function [slowSampling] = get.slowSampling(CT)
      if CT.prf > 20
        % samplingPeriod in us
        slowSampling = false;
      else
        % samplingPeriod in ms
        slowSampling = true;
      end
    end
    % --------------------------------------------------------------------------
    function [samplingPeriod] = get.samplingPeriod(CT)
      if CT.slowSampling
        % samplingPeriod in ms
        samplingPeriod = uint16(1./CT.prf*1e3);
      else
        % samplingPeriod in us
        samplingPeriod = uint16(1./CT.prf*1e6);
      end
    end

  end

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods (Access = private)
  end

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  methods % set / get methods
    function set.mode(CT,inMode)
      if strcmp(inMode,'all')
        CT.mode = 'all'; % trigger on all channels at the same time
      else
        CT.mode = validate_mode(inMode); % normal triggering
      end
    end

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
