#pragma once

#include <Wire.h>
#include <SerLCD.h>
#include <Network.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SimpleSoftTimer.h>

#define UTC_OFFSET_SECONDS 3600 // UTC+1

using namespace HolisticSolutions;

extern SerLCD lcd;
extern SimpleSoftTimer displayTimer;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern int readAxisY;
extern int readAxisX;
extern int brightness;
extern int tempDiff;

void lcdSetup();

void printTimeAndDate();

void printTempAndCO2();

void clearDisplay();
