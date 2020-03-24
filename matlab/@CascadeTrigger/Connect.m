% function [] = Connect(CT)
% Johannes Rebling, (johannesrebling@gmail.com), 2019

function [] = Connect(CT)
  t1 = tic;
  if ~isempty(CT.serialPtr) && CT.isConnected
    CT.VPrintF('[CT] Trigger already connected!\n');
  else
    CT.VPrintF('[CT] Connecting to trigger...');
    try
      CT.serialPtr = openPort(CT.SERIAL_PORT,CT.BAUD_RATE);
      CT.isConnected = true;
      CT.Done(t1);
    catch
      drawnow();
      CT.isConnected = false;
      CT.Verbose_Warn('Opening serial connection failed!');
      CT.Verbose_Warn('Is the white USB cable (red tape) connected?\n');
    end
  end
end
