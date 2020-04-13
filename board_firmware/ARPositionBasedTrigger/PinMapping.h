/*
	File: PinMapping.h
	Author: Urs Hofmann
	Mail: hofmannu@biomed.ee.ethz.ch
	Date: 13.04.2019
	Description: Pin mapping for quadrature decoder board

*/

#ifndef PINMAPPING_H
#define PINMAPPING_H

const unsigned char STATUS_LEDS[4] = {27, 28, 29, 30};
const unsigned char TRIG_PINS[3] = {15, 18, 19};

const unsigned char HCTL_CLOCK_PIN = 10;
const unsigned char HCTL_RST_PIN = 11;
const unsigned char HCTL_OE_PIN = 12;
const unsigned char HCTL_SEL_PIN = 24;
const unsigned char HCTL_MANUAL_RESET_PIN = 25; // push button connected here
const unsigned char TRIG_COUNT_RESET = 26; // push button connected here

#endif