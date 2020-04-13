function [cMap] = make_linear_colormap(lowColor,highColor,nColors,invert)
  % [cMap] = make_linear_colormap(lowColor,highColor);
  % [cMap] = make_linear_colormap(lowColor,highColor,nColors,invert);
  % lowColor = rgb triplet, defining the color corresponding to low values
  % highColor = rgb triplet, defining the color corresponding to high values
  % optional arguments:
  %   nColors = number of colors used for map, default 255
  %   invert = invert colormap, defautl false

  % Example:
  % figure;
  % imagesc(peaks);
  % dark = [0 0 0];
  % red = [255 0 0];
  % [cMap] = make_linear_colormap(dark,red);
  % colormap(cMap);
  % colorbar();

  allColors = [lowColor; highColor];
  % if sum(any(allColors>1)) || sum(any(allColors<0))
  %   error('Colors have to be in the 0-1 range!')
  % end

  if nargin == 2
    nColors = 255;
    invert = false;
  end

  if nargin == 3
    invert = false;
  end
  cMap(:,1) = linspace(lowColor(1),highColor(1),nColors);
  cMap(:,2) = linspace(lowColor(2),highColor(2),nColors);
  cMap(:,3) = linspace(lowColor(3),highColor(3),nColors);
  cMap = cMap./max(cMap(:)); % normalize to 0->1 range

  % NOTE colormaps defined here use different definition than normal matlab colormaps
  % lowcolor defines the "end" of classical colormaps, i.e. what will be displayed
  % for low values
  % thats why we flip them here by default
  if ~invert
    cMap = flipud(cMap);
  end

end
