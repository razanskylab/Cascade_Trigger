% File: StopN.m @ CascadeCommunicator
% Author: Urs Hofmann, Weiye Li
% Mail: weiye.li@uzh.ch
% Date: Feb. 17th, 2022

% Description: Stops cascader when triggering in N-events mode, and reads actual number of triggers

function StopN(cc)

  % wait for 4 bytes of uint32 number of trigger counts
  t1 = tic;
  while(cc.S.NumBytesAvailable < 4)
    if toc(t1) > 1
      error("[Cascader] Teensy timeout!\n");
    end
    % do NOTHING
  end

  lastCascCount = read(cc.S, 1, 'uint32');
  if isfinite(lastCascCount)
    cc.lastCascCount = lastCascCount;
  end
  
  cc.Handshake();
  fprintf("done after %i trigger event(s)!\n", lastCascCount);

end