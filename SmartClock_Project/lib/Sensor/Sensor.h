#pragma once
#include <SerLCD.h>
#include <SimpleSoftTimer.h>

using namespace HolisticSolutions;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern SerLCD lcd;
extern SimpleSoftTimer displayTime;

void printTempAndCO2();