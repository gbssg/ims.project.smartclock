#include <Wire.h>
#include "SparkFun_ENS160.h"
#include "SparkFunBME280.h"
#include <SparkFun_Qwiic_OLED.h>
#include <SerLCD.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>
#include <ESP32Time.h>
#include <time.h>
#include <ClockProvider.h>
#include <SparkFun_Qwiic_Button.h>
#include <SimpleSoftTimer.h>
#include "LCD.h"

using namespace HolisticSolutions;

int ppm;
int tempDiff = 4;
float temp;
int brightness = 100;
int highPPM = 1000;
int midPPM = 800;
int highTemp = 28;
bool buzzerMuted = false;
ESP32Time rtc;
QwiicBuzzer buzzer;
SparkFun_ENS160 ens160;
BME280 bme280;
SerLCD lcd;
QwiicButton button;
// SimpleSoftTimer heatDisplaytimer(200);

byte smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000};

byte neutral[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000};

byte frownie[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

byte tot[8] = {
    0b00000,
    0b00000,
    0b10101,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

void printTemp()
{
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp, 2);
  lcd.println(" C   ");
}

void printCO2()
{
  lcd.setCursor(0, 0);
  if (ppm > highPPM)
  {

    lcd.print("CO2 : ");
  }
  else if (ppm <= highPPM)
  {
    lcd.print("CO2 :  ");
  }

  lcd.print(ppm);
  lcd.print(" ppm");
  lcd.setCursor(15, 0);
  if (ppm <= midPPM)
  {
    lcd.writeChar(0);
  }
  else if (ppm <= highPPM)
  {
    lcd.writeChar(1);
  }
  else if (ppm > highPPM)
  {
    lcd.writeChar(2);
  }
}

void buzz()
{
  if(ppm > midPPM)
  {
    if(!buzzerMuted)
    {
      button.LEDoff();
      buzzer.configureBuzzer(2730, 1000, SFE_QWIIC_BUZZER_VOLUME_MIN);
      buzzer.on();
      printCO2();
      printTemp();
    }
    
    if (button.isPressed())
    {
      buzzer.off();
      buzzerMuted = true;
    }
  }
  else if (ppm < (midPPM - 100))
  {
    buzzerMuted = false;
  }
}

void CP_getTimeAndDate()
{
  Serial.print(CP_getHourAsString());
  Serial.print(":");
  Serial.print(CP_getMinuteAsString());
  Serial.print(":");
  Serial.println(CP_getSecondAsString());

  Serial.print(CP_getDayAsString());
  Serial.print(".");
  Serial.print(CP_getMonthAsString());
  Serial.print(".");
  Serial.println(CP_getYearAsString());
  Serial.println("");
}

void setup()
{

  Serial.begin(115200);
  CP_init(0);
  Wire.begin();

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

  Wire1.begin();
  lcd.begin(Wire1);
  lcd.setAddress(0x72);
  lcd.clear();
  lcd.setContrast(50);

  lcd.createChar(0, smiley);
  lcd.createChar(1, neutral);
  lcd.createChar(2, frownie);
  lcd.createChar(3, tot);

  button.LEDoff();
}

void loop()
{
  CP_getTimeAndDate();

  ppm = ens160.getECO2();
  temp = bme280.readTempC() - tempDiff;
  printCO2();
  printTemp();

  buzz();

  if (button.isPressed())
  {
    button.LEDon(brightness);
  }
  else
  {
    button.LEDoff();
  } 
}
