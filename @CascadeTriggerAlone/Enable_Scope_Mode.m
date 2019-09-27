% function [] = Enable_Scope_Mode(AQ)
% Johannes Rebling, (johannesrebling@gmail.com), 2019

function [] = Enable_Scope_Mode(CT,nTrigger)
  % starts recording of the calibration data in the teensy
  %  triggerPeriod = serial_read_16bit();
  %  nTrigger = serial_read_16bit(); // trigger how many times?
  if nargin == 1
    nTrigger = 0; % default scope mode, i.e. free running
  end

  triggerPeriod = uint16(CT.samplingPeriod);
  nTrigger = uint16(nTrigger);
  slowMode = uint16(CT.slowSampling);

  if (nTrigger == 0)
    CT.PrintF('[CT] Enabling free-running trigger @ %2.2fkHz.\n',CT.prf*1e-3);
  else
    CT.PrintF('[CT] Enabling %i trigger @ %2.2fkHz.\n',nTrigger,CT.prf*1e-3);
  end

  CT.Write_Command(CT.ENABLE_SCOPE_MODE);
  CT.Write_16Bit(slowMode);
  CT.Write_16Bit(triggerPeriod);
  CT.Write_16Bit(nTrigger);
end
