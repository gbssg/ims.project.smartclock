#include <Wire.h>
#include <SerLCD.h>
#include "LCD.h"

SerLCD lcd;

void lcdSetup()
{
    Wire1.begin();
    lcd.begin(Wire1);
    lcd.setAddress(0x72);
    lcd.clear();
    lcd.setContrast(50);
}