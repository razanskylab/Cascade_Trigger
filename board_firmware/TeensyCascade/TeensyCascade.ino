// File: TeensyCascade.ino
// Author: Urs Hofmann
// Mail: hofmannu@biomed.ee.ethz.ch
// Date: 25.04.2019

// Description: Switched to minimal main file, moved stuff to class

#include "Cascader.h"

Cascader myCascader;

void setup()
{
	myCascader.setup();
}

void loop()
{
	myCascader.operate();
}