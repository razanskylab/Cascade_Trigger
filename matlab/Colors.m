classdef Colors < handle
  properties (Constant = true)
    nColors = 256;

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % define some pretty colors for plotting -------------------------------------
    PureRed = [1.0, 0.0, 0.0];
    DarkRed = [150,0,0]./255;
    BloodRed = [187, 10, 30]*0.8./255;
    % my own simple colors
    Yellowish = [0.9, 0.9, 0.0];
    DarkOrange = [1.0, 0.5, 0.0];
    LightOrange = [1.0, 0.7, 0.3];

    DarkGreen = [0.0, 0.6, 0.0];
    BrightGreen = [0.0, 1.0, 0.0];
    LightGreen = [0.0, 0.8, 0.0];

    DarkPurple = [0.5, 0.0, 0.5];
    DarkBlue = [103,169,207]./2./255;
    DarkishBlue = [5,100,255]./255;
    PureBlue = [0,0,255]./255;
    DarkViolett = [100,0,55]./255;

    LightGray = [255, 255, 255]*0.75./255;
    MediumGray = [255, 255, 255]*0.5./255;
    DarkGray = [255, 255, 255]*0.25./255;

    Black = [0, 0, 0];
    White = [1, 1, 1];

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % fancy color families from:
  % http://www.flatuicolorpicker.com/category/all

    % REDS from http://www.flatuicolorpicker.com/category/red
    cinnabar = [240,52,52]./255;
    softRed = [236,100,75]./255;
    chestnutRose = [210, 77, 87]./255;
    pomegranate = [242, 38, 19]./255;
    thunderbird = [217, 30, 24]./255;
    oldBrick = [150, 40, 27]./255;
    flamingo = [239, 72, 54]./255;
    valencia = [214, 69, 65]./255;
    tallPoppy = [192, 57, 43]./255;
    monza = [207, 0, 15]./255;
    scarlet = [242, 38, 19]./255;

    % GREEN from http://www.flatuicolorpicker.com/category/green
    malachite = [0,230,64]./255;
    summerGreen = [145,180,150]./255;
    mediumTurquoise = [78,205,196]./255;
    aquaIsland = [162, 222, 208]./255;
    gossip = [ 135, 211, 124]./255;
    darkSeaGreen = [ 144, 198, 149]./255;
    eucalyptus = [ 38, 166, 91]./255;
    caribbeanGreeN = [ 3, 201, 169]./255;
    silverTree = [ 104, 195, 163]./255;
    downy = [ 101, 198, 187]./255;
    mountainMeadow = [ 27, 188, 155]./255;
    lightSeaGreeN = [ 27, 163, 156]./255;
    mediumAquamarinE = [ 102, 204, 153]./255;
    turquoise = [ 54, 215, 183]./255;
    madang = [ 200, 247, 197]./255;
    riptide = [ 134, 226, 213]./255;
    shamrock = [ 46, 204, 113]./255;
    niagara1 = [ 22, 160, 133]./255;
    emerald = [63, 195, 128]./255;
    greenHaze = [1, 152, 117]./255;
    freeSpeechAquamarine = [3, 166, 120]./255;
    oceanGreen = [77, 175, 124]./255;
    niagara2 = [42, 187, 155]./255;
    jade = [0, 177, 106]./255;
    salem = [30, 130, 76]./255;
    observatory = [4, 147, 114]./255;
    jungleGreen = [38, 194, 129]./255;

    % BLUEs from http://www.flatuicolorpicker.com/category/blue
    sherpaBlue = [1,50,67]./255;
    sanMarino = [68,108,179]./255;
    aliceBlue = [228, 241, 254]./255;
    royalBlue = [65, 131, 215]./255;
    pictonBlue = [89, 171, 227]./255;
    spray = [129, 207, 224]./255;
    shakespeare = [82, 179, 217]./255;
    hummingBird = [197, 239, 247]./255;
    curiousBlue = [52, 152, 219]./255;
    madison = [44, 62, 80]./255;
    dodgerBlue = [25, 181, 254]./255;
    ming = [51, 110, 123]./255;
    ebonyClay = [34, 49, 63]./255;
    malibu = [107, 185, 240]./255;
    summerSky = [30, 139, 195]./255;
    chambray =  [58, 83, 155]./255;
    pickledBluewood = [52, 73, 94]./255;
    hoki = [103, 128, 159]./255;
    jellyBean = [37, 116, 169]./255;
    jacksonsPurple = [31, 58, 147]./255;
    jordyBlue = [137, 196, 244]./255;
    steelBlue = [75, 119, 190]./255;
    fountainBlue = [92, 151, 191]./255;

    % ORANGES from  http://www.flatuicolorpicker.com/category/orange
    saffronMango= [250,190,88]./255;
    confetti= [233,212,96]./255;
    capeHoney= [253, 227, 167]./255;
    california= [248, 148, 6]./255;
    fireBush= [235, 149, 50]./255;
    tahitiGold= [232, 126, 4]./255;
    casablanca= [244, 179, 80]./255;
    crusta= [242, 120, 75]./255;
    seaBuckthorn= [235, 151, 78]./255;
    lightningYellow= [245, 171, 53]./255;
    burntOrange= [211, 84, 0]./255;
    buttercup= [243, 156, 18]./255;
    ecstasy= [249, 105, 14]./255;
    sandstorm= [249, 191, 59]./255;
    jaffa= [242, 121, 53]./255;
    zest= [230, 126, 34]./255;

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % linear colormaps comprised of two colors
    whiteToRed = make_linear_colormap(Colors.White,Colors.PureRed,Colors.nColors);
    whiteToBlue = make_linear_colormap(Colors.White,Colors.DarkBlue,Colors.nColors);
    whiteToGreen = make_linear_colormap(Colors.White,Colors.DarkGreen,Colors.nColors);

    blackToRed = make_linear_colormap(Colors.Black,Colors.PureRed,Colors.nColors);
    blackToBlue = make_linear_colormap(Colors.Black,Colors.DarkBlue,Colors.nColors);
    blackToGreen = make_linear_colormap(Colors.Black,Colors.DarkGreen,Colors.nColors);

    greenToRed = make_linear_colormap(Colors.BrightGreen,Colors.PureRed,Colors.nColors);
    redToGreen = flipud(Colors.greenToRed);

    blueToRed = make_linear_colormap(Colors.PureBlue,Colors.PureRed,Colors.nColors);
    redtoBlue = make_linear_colormap(Colors.PureRed,Colors.PureBlue,Colors.nColors);

    blueToRedDark = make_linear_colormap(Colors.DarkBlue,Colors.DarkRed,Colors.nColors);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % linear colormaps based on flat ui color picker colors
    greenToRedFUI = make_linear_colormap(Colors.malachite,Colors.monza,Colors.nColors);
    redToGreenFUI = flipud(Colors.greenToRedFUI);

    greenToOrangeFUI = make_linear_colormap(Colors.malachite,Colors.ecstasy,Colors.nColors);
    orangeToGreenFUI = flipud(Colors.greenToOrangeFUI);

    redOrangeGreenFUI = make_linear_triple_colormap(Colors.pomegranate,Colors.ecstasy,Colors.malachite,Colors.nColors);
    greenOrangeRedFUI = flipud(Colors.redOrangeGreenFUI);

    blueOrangeGreenFUI = make_linear_triple_colormap(Colors.pictonBlue, Colors.ecstasy, Colors.malachite, Colors.nColors);
    blueGreenOrangeFUI = make_linear_triple_colormap(Colors.pictonBlue, Colors.malachite, Colors.ecstasy, Colors.nColors);
    greenOrangeBlueFUI = make_linear_triple_colormap(Colors.malachite, Colors.ecstasy, Colors.pictonBlue, Colors.nColors);
    greenBlueOrangeFUI = make_linear_triple_colormap(Colors.malachite, Colors.pictonBlue, Colors.ecstasy, Colors.nColors);
    orangeGreenBlueFUI = make_linear_triple_colormap(Colors.ecstasy, Colors.malachite, Colors.pictonBlue, Colors.nColors);
    orangeBlueGreenFUI = make_linear_triple_colormap(Colors.ecstasy, Colors.pictonBlue, Colors.malachite, Colors.nColors);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % linear colormaps composed of three colors
    redRedBlue = make_linear_triple_colormap(Colors.DarkRed,Colors.PureRed,Colors.PureBlue,Colors.nColors);
    whiteRedWhite = make_linear_triple_colormap(Colors.White,Colors.PureRed,Colors.White,Colors.nColors)

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % colormaps based on
    % http://www.kennethmoreland.com/color-maps/ColorMapsExpanded.pdf
    divMapRedtoBlue = diverging_map(linspace(0,1,Colors.nColors),Colors.DarkRed,Colors.DarkBlue);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % all fancy reds
    redColorMap = [Colors.cinnabar;Colors.softRed;Colors.chestnutRose;...
      Colors.pomegranate;Colors.thunderbird; Colors.oldBrick;...
      Colors.flamingo; Colors.valencia; Colors.tallPoppy; Colors.monza];

    greenColorMap = [Colors.malachite; Colors.summerGreen; Colors.mediumTurquoise; Colors.aquaIsland;...
      Colors.gossip; Colors.darkSeaGreen; Colors.eucalyptus; Colors.caribbeanGreeN;...
      Colors.silverTree; Colors.downy; Colors.mountainMeadow; Colors.lightSeaGreeN;...
      Colors.mediumAquamarinE; Colors.turquoise; Colors.madang; Colors.riptide;...
      Colors.shamrock; Colors.niagara1; Colors.emerald; Colors.greenHaze;...
      Colors.freeSpeechAquamarine; Colors.oceanGreen; Colors.niagara2; Colors.jade;...
      Colors.salem; Colors.observatory; Colors.jungleGreen];


    blueColorMap = [Colors.sherpaBlue; Colors.sanMarino; Colors.aliceBlue; Colors.royalBlue; ...;
      Colors.pictonBlue; Colors.spray; Colors.shakespeare; Colors.hummingBird; ...
      Colors.curiousBlue; Colors.madison; Colors.dodgerBlue; Colors.ming; ...
      Colors.ebonyClay; Colors.malibu; Colors.summerSky; Colors.chambray; ...
      Colors.pickledBluewood; Colors.hoki; Colors.jellyBean; Colors.jacksonsPurple; ...
      Colors.jordyBlue; Colors.steelBlue; Colors.fountainBlue;]

    orangeColorMap = [Colors.saffronMango; Colors.confetti; Colors.capeHoney; Colors.california; ...
      Colors.fireBush; Colors.tahitiGold; Colors.casablanca; Colors.crusta; ...
      Colors.seaBuckthorn; Colors.lightningYellow; Colors.burntOrange; ...
      Colors.buttercup; Colors.ecstasy; Colors.sandstorm; Colors.jaffa; Colors.zest]

  end

end
