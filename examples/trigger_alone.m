if exist('CT','var')
  fprintf('Using existing CT!\n')
else
  CT = CascadeTriggerAlone;
end
CT.prf = 100; % set trigger freq.
CT.mode = 'us'; % us dye onda32
CT.Setup_Trigger();

% scope has two modes, continous:
CT.Enable_Scope_Mode();
pause(2);
CT.Disable_Scope();

% scope has two modes, nTrigger:
nTrigger = 100;
CT.Enable_Scope_Mode(nTrigger);
pause(nTrigger./CT.prf);
CT.Disable_Scope();
