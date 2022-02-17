% File: Set_Trigger_Type.m @ CascadeCommunicator
% Author: Weiye Li, Urs Hofmann
% Mail: weiye.li@uzh.ch
% Date: Feb. 17th, 2022

% Description: set trigger type as either:
% - 0: triggers upon both rising and falling edges
% - 1: triggers upon rising edges ONLY

function Set_Trigger_Type(cc, trigType)

  if nargin == 1
    error('[Cascader] must set trigger type! 0 means rising and falling edges, 1 means rising edge ONLY.\n');
  elseif nargin == 2
    if trigType == 0
      fprintf('[Cascader] set trigger type to BOTH rising and falling edges.\n')';
      cc.trigType = trigType;
    elseif trigType == 1
      fprintf('[Cascader] set trigger type to ONLY rising edges.\n');
      cc.trigType = trigType;
    else
      error('[Cascader] invalid trigger type!\n');
    end
  else
    error('[Cascader] invalid number of input arguments when setting trigger type!\n');
  end

end