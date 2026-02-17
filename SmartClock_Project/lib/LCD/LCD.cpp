#include <Wire.h>
#include <SerLCD.h>
#include "LCD.h"
#include <Network.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SimpleSoftTimer.h>

#define UTC_OFFSET_SECONDS 3600 // UTC+1

extern SimpleSoftTimer displayTimer;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
SerLCD lcd;

int ppm;
float temp;
int highTemp = 28;
int highPPM = 1000;
int midPPM = 800;
int readAxisY;
int readAxisX;
int brightness = 100;
int tempDiff = 5;

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

// Start Symbol für das LCD
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

    // printCO2
    lcd.setCursor(0, 0);
    lcd.print("CO2 :");

    if (ppm >= highPPM)
    {
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.setCursor(15, 0);
      lcd.writeChar(2);
    }
    else if (ppm < highPPM && ppm > midPPM)
    {
      lcd.print(" ");
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.setCursor(15, 0);
      lcd.writeChar(1);
    }
    else if (ppm <= midPPM)
    {
      lcd.print(" ");
      lcd.print(ppm);
      lcd.print(" ppm ");
      lcd.setCursor(15, 0);
      lcd.writeChar(0);
    }

    // printTemp
    lcd.setCursor(0, 1);
    lcd.print("Temp:  ");
    lcd.print(temp, 1);
    lcd.setCursor(12, 1);
    lcd.write(223);
    lcd.print("C");

    displayTimer.restart();
  }
}
