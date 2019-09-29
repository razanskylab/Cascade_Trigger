function [trigOutChMask,triggerOut] = Setup_Trigger(CT)
  CT.VPrintF('[CT] Setting trigger channel: ');
  CT.Write_Command(CT.SET_TRIGGER_CH); % same as command, but lets not confuse our users...
  switch CT.mode
    case 'all'
      CT.VPrintF(' all channels...');
      trigMode = CT.ALL_TRIG;
    case 'us'
      CT.VPrintF(' us...');
      trigMode = CT.US_TRIG;
    case 'dye'
      CT.VPrintF(' dye...');
      trigMode = CT.EDGE_TRIG; % same as command, but lets not confuse our users...
    case 'onda32'
      CT.VPrintF(' onda32...');
      trigMode = CT.ONDA_TRIG; % same as command, but lets not confuse our users...
    otherwise
      CT.Verbose_Warn(sprintf('Trigger mode %s not supported. Using US!\n', CT.mode));
      CT.VPrintF(' us...');
      trigMode = CT.US_TRIG; % same as command, but lets not confuse our users...
  end

  CT.Write_Command(trigMode); % same as command, but lets not confuse our users...
  [~,trigOutChMask] = CT.Read_Data(2); % read trigger out
  [~,triggerOut] = CT.Read_Data(2); % read trigger out
  if triggerOut ~= trigMode
    CT.Verbose_Warn('Setting trigger mode failed!');
  end
  CT.Wait_Done();
  CT.Done();
end
