#include <Wire.h>
#include <SerLCD.h>
#include "LCD.h"
#include <Network.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SimpleSoftTimer.h>

#define UTC_OFFSET_SECONDS 3600 // UTC+1
using namespace HolisticSolutions;

extern SimpleSoftTimer displayTime;
extern SerLCD lcd;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;

// Smiley Symbole für das LCD Display
byte smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000};

// Neutraler Smiley für das LCD Display
byte neutral[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000};
    
// Frownie Smiley für das LCD Display
byte frownie[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

// Totenkopf Smiley für das LCD Display
byte tot[8] = {
    0b00000,
    0b00000,
    0b10101,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

// Funktion für die Initialisierung des LCD Displays
void lcdSetup()
{
  Wire1.begin();
  lcd.begin(Wire1);
  lcd.setAddress(0x72);
  lcd.clear();
  lcd.setContrast(50);

  lcd.createChar(0, smiley);
  lcd.createChar(1, neutral);
  lcd.createChar(2, frownie);
  lcd.createChar(3, tot);
}

// Funktion für die Anzeige von der Zeit und vom Datum
void printTimeAndDate()
{
  lcd.setCursor(0, 0);
  lcd.print("Time:   ");
  lcd.print(timeClient.getFormattedTime());

  lcd.setCursor(0, 1);
  lcd.print("Date:   ");
  lcd.print(timeClient.getFormattedDate());
}

// Funktion für die Anzeige von der Temperatur und vom CO2-Gehalt
void printTempAndCO2()
{
  if (displayTime.isTimeout()) 
  {
    // printTemp
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temp, 2);
    lcd.println(" C   ");

    // printCO2
    lcd.setCursor(0,0);
    if (ppm >= highPPM)
    {
      lcd.print("CO2 : ");
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.writeChar(2);
    }
    else if (ppm < highPPM && ppm > midPPM )
    {
      lcd.print("CO2 :  ");
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.writeChar(1);
    }
    else if (ppm <= midPPM)
    {
      lcd.print("CO2 :  ");
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.writeChar(0);
    }      
    displayTime.restart();
  }
}