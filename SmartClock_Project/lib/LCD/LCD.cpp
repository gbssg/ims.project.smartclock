#include <Wire.h>
#include <SerLCD.h>
#include "LCD.h"
#include <Network.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SimpleSoftTimer.h>

#define UTC_OFFSET_SECONDS 3600 // UTC+1
using namespace HolisticSolutions;

extern SimpleSoftTimer displayTimer;
extern SerLCD lcd;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern int readAxisY;
extern int readAxisX;

// Smiley Symbole für das LCD
byte smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000};

// Neutraler Smiley für das LCD
byte neutral[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000};

// Frownie Smiley für das LCD
byte frownie[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

// Totenkopf Smiley für das LCD
byte death[8] = {
    0b00000,
    0b00000,
    0b10101,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

// Pfeil Symbol für das LCD
byte arrow[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000};

byte start[8] = {
    0b00000,
    0b01100,
    0b01110,
    0b01111,
    0b01111,
    0b01110,
    0b01100,
    0b00000};

// Funktion für die Initialisierung des LCD
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
  lcd.createChar(3, death);
  lcd.createChar(4, arrow);
  lcd.createChar(5, start);
}

// Funktion für die Anzeige von der Zeit und vom Datum
void printTimeAndDate()
{
  if (displayTimer.isTimeout())
  {
    lcd.setCursor(0, 0);
    lcd.print("Time:   ");
    lcd.print(timeClient.getFormattedTime());

    lcd.setCursor(0, 1);
    lcd.print("Date:   ");
    lcd.print(timeClient.getFormattedDate());
    displayTimer.restart();
  }
}

// Funktion für die Anzeige von der Temperatur und vom CO2-Gehalt
void printTempAndCO2()
{
  if (displayTimer.isTimeout())
  {
    // printTemp
    lcd.setCursor(0, 1);
    lcd.print("Temp:  ");
    lcd.print(temp, 2);
    lcd.println(" C   ");

    // printCO2
    lcd.setCursor(0, 0);
    if (ppm >= highPPM)
    {
      lcd.print("CO2 : ");
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.writeChar(2);
    }
    else if (ppm < highPPM && ppm > midPPM)
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
    displayTimer.restart();
  }
}

void readAxes()
{
  if (displayTimer.isTimeout())
  {
    Serial.print("Y-Axis: ");
    Serial.println(readAxisY);
    Serial.print("X-Axis: ");
    Serial.println(readAxisX);
    displayTimer.restart();
  }
}