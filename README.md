# Cascade Trigger

This library was originally written to allow multiwavelength imaging in optoacoustic microscopy. After a position trigger coming from the Quadrature decoder, this library will make a burst mode trigger scheme allowing the acquisition of multiwavelength OA images within a single mechanical overfly scan. We provide a convenient MATLAB interface to define the required number of averages and number of wavelengths.

The code was tested with Teensy 3.6 and Teensy 4.1 and should be compatible with both. To programm the controller you need to download and install [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html).

Depending on how you use the library, you might need to adapt a few ports.

## Literature

*  Urs A. T. Hofmann, Johannes Rebling, Héctor Estrada, Pavel Subochev, Daniel Razansky: Rapid functional optoacoustic micro-angiography in a burst mode, Optics Letters 45 (9) 2020, DOI: [10.1364/OL.387630](https://doi.org/10.1364/OL.387630)