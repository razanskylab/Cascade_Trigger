function [cMap] = make_linear_triple_colormap(lowColor,middleColor,highColor,nColors,invert)
  % [cMap] = make_linear_colormap(lowColor,highColor);
  % [cMap] = make_linear_colormap(lowColor,highColor,nColors,invert);
  % creates a linear colormap between lowColor, highColor
  % lowColor = rgb triplet, defining the color corresponding to low values
  % highColor = rgb triplet, defining the color corresponding to high values
  % optional arguments:
  %   nColors = number of colors used for map, default 255
  %   invert = invert colormap, defautl false

  % Example:
  % figure;
  % imagesc(peaks);
  % blue = [0 0 255];
  % white = [255 255 255];
  % red = [255 0 0];
  % [cMap] = make_linear_triple_colormap(blue,white,red);
  % colormap(cMap);
  % colorbar();

  
  allColors = [lowColor; middleColor; highColor];
  if sum(any(allColors>1)) || sum(any(allColors<0))
    error('Colors have to be in the 0-1 range!')
  end

  if nargin < 5
    nColors = 255;
    invert = false;
  end

  if nargin < 3
    invert = false;
  end

  if ~rem(nColors,2)
    nColors = nColors+1;
  end

  half = ceil(nColors/2);
  lowHalf = 1:half; % eg. 1:128
  highHalf = half:nColors; % eg. 128:255

  cMap = zeros(nColors,3);
  cMap(lowHalf,1) = linspace(lowColor(1),middleColor(1),half);
  cMap(lowHalf,2) = linspace(lowColor(2),middleColor(2),half);
  cMap(lowHalf,3) = linspace(lowColor(3),middleColor(3),half);

  cMap(highHalf,1) = linspace(middleColor(1),highColor(1),half);
  cMap(highHalf,2) = linspace(middleColor(2),highColor(2),half);
  cMap(highHalf,3) = linspace(middleColor(3),highColor(3),half);

  % cMap = cMap./max(cMap(:)); % normalize to 0->1 range
  if ~invert
    cMap = flipud(cMap);
  end

end
