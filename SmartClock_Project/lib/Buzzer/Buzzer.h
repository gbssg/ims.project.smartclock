# pragma once

#include <SerLCD.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>
#include <SparkFun_Qwiic_Button.h>
#include "LCD.h"

extern QwiicBuzzer buzzer;
extern SerLCD lcd;
extern QwiicButton button;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern bool buzzerMuted;
extern bool buzzerBuzzing;

void warnBuzz();