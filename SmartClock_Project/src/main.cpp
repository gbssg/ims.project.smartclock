#include <Wire.h>
#include "SparkFun_ENS160.h"
#include "SparkFunBME280.h"
#include <SparkFun_Qwiic_OLED.h>
#include <SerLCD.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>
#include <SparkFun_Qwiic_Button.h>
#include <SimpleSoftTimer.h>
#include "LCD.h"
#include <Network.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Menu.h>
#include <Buzzer.h>
#include <Timer.h>

#define JoyStick_Y_Pin A0
#define JoyStick_X_Pin A1

using namespace HolisticSolutions;

SparkFun_ENS160 ens160;
BME280 bme280;

void setup()
{
  pinMode(JoyStick_X_Pin, INPUT);
  pinMode(JoyStick_Y_Pin, INPUT);

  Serial.begin(115200);
  Wire.begin();
  timerInit();
  connectWifi();
  timeClient.begin();
  analogReadResolution(7);

  if (!ens160.begin())
  {
    Serial.println("ENS160 Hat nicht geantwortet.");
    while (1)
      ;
  }

  if (!bme280.beginI2C())
  {
    Serial.println("BME280 Hat nicht geantwortet.");
    while (1)
      ;
  }

  if (buzzer.begin() == false)
  {
    Serial.println("Buzzer Hat nicht geantwortet.");
    while (1)
      ;
  }

  if (button.begin() == false)
  {
    Serial.println("Button hat nicht geantwortet.");
    while (1)
      ;
  }

  if (ens160.setOperatingMode(SFE_ENS160_RESET))
    Serial.println("Ready.");

  ens160.setOperatingMode(SFE_ENS160_STANDARD);

  lcdSetup();
  button.LEDoff();
}

void loop()
{
  // Joystick Werte einlesen
  readAxisY = analogRead(JoyStick_Y_Pin);
  readAxisX = analogRead(JoyStick_X_Pin);

  // NTP Client updaten
  timeClient.update();

  // CO2 Wert und Temperatur auslesen
  ppm = ens160.getECO2();
  temp = bme280.readTempC() - tempDiff;

  // Menü-Funktion aufrufen
  manageMenu();

  // Buzzer Funktion aufrufen
  warnBuzz();

  // LED vom Button steuern
  button.isPressed() ? button.LEDon(brightness) : button.LEDoff();

  startTimer();
}
