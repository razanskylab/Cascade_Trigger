% function [] = Enable_Scope_Mode(AQ)
% Johannes Rebling, (johannesrebling@gmail.com), 2019

function [] = Enable_Scope_Mode(CT)
  triggerFreq = uint16(CT.prf);
  % triggerPeriod = uint16(CT.samplingPeriod);
  nTrigger = uint16(CT.aodTrigger);
  postAcqDelay = uint16(CT.postAcqDelay); % set acq. delay in microseconds

  if (nTrigger == 0)
    CT.PrintF('[CT] Enabling free-running trigger @ %2.2fkHz.\n',CT.prf*1e-3);
  else
    CT.PrintF('[CT] Enabling %i trigger @ %2.2fkHz.\n',nTrigger,CT.prf*1e-3);
  end

  CT.Write_Command(CT.ENABLE_SCOPE_MODE);
  CT.Write_16Bit(nTrigger);
  CT.Write_16Bit(triggerFreq);
  CT.Write_16Bit(postAcqDelay);
end
