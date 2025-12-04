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

#define UTC_OFFSET_SECONDS 3600 // UTC+1

#define JoyStick_Y_Pin A0
#define JoyStick_X_Pin A1

using namespace HolisticSolutions;

// Variabeln definieren
bool buttonRelease = false;
int ppm;
int tempDiff = 5;
float temp;
int brightness = 100;
int highPPM = 1000;
int midPPM = 800;
int highTemp = 28;
bool buzzerMuted = false;
bool buzzerBuzzing = true;
int readAxisY;
int readAxisX;
int runThrough = 0;
enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE,
  TIMER_STATE
};

menuState currentState;
menuState lastState = CLOCK_STATE;
bool displayCleared = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_SECONDS);

// Objekte erstellen
QwiicBuzzer buzzer;
SparkFun_ENS160 ens160;
BME280 bme280;
SerLCD lcd;
QwiicButton button;
SimpleSoftTimer displayTime(100);

// Setup Funktion
void setup()
{
  pinMode(JoyStick_X_Pin, INPUT);
  pinMode(JoyStick_Y_Pin, INPUT);

  Serial.begin(115200);
  Wire.begin();
  displayTime.start(100);
  connectWifi();
  timeClient.begin();
  analogReadResolution(10);

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

// Loop Funktion
void loop()
{
  // Joystick Werte einlesen
  readAxisY = analogRead(JoyStick_Y_Pin);
  readAxisX = analogRead(JoyStick_X_Pin);

  // NTP Client updaten
  timeClient.update();
  
  // CO2 Wert und Temperatur auslesend
  ppm = ens160.getECO2();
  temp = bme280.readTempC() - tempDiff;

  // Menü-Funktion aufrufen
  manageMenu();
  
  // Buzzer Funktion aufrufen
  warnBuzz();

  // Temperatur und CO2 Gehalt anzeigen
  printTempAndCO2();

  // LED vom Button steuern
  button.isPressed() ? button.LEDon(brightness) : button.LEDoff();

}
