#include <Wire.h>
#include "SparkFun_ENS160.h"
#include "SparkFunBME280.h"
#include <SparkFun_Qwiic_OLED.h>
#include <SerLCD.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>

QwiicBuzzer buzzer;
SparkFun_ENS160 ens160;
BME280 bme280;
SerLCD lcd;
int ppm;
int tempDiff = 4;
float temp;

int startHour = 12;
int startMinute = 22;
int startSecond = 20;

long lastMillis = 0;
long totalSeconds = 0;

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

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  if (!ens160.begin())
  {
    Serial.println("ENS160: Hat nicht geantwortet.");
    while (1)
      ;
  }

  if (!bme280.beginI2C())
  {
    Serial.println("BME280: Hat nicht geantwortet.");
    while (1)
      ;
  }

  if (buzzer.begin() == false)
  {
    Serial.println("Buzzer Hat nicht geantwortet.");
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

  totalSeconds = startHour * 3600 + startMinute * 60 + startSecond;
}

void printTemp()
{
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp, 2);
  lcd.println(" C");
  delay(200);
}

void printCO2()
{
  lcd.setCursor(0, 0);
  if (ppm > 1000)
  {

    lcd.print("CO2 : ");
  }
  else if (ppm <= 1000)
  {
    lcd.print("CO2 :  ");
  }

  lcd.print(ppm);
  lcd.print(" ppm");
  lcd.setCursor(15, 0);
  if (ppm <= 600)
  {
    lcd.writeChar(0);
  }
  else if (ppm <= 1000)
  {
    lcd.writeChar(1);
  }
  else if (ppm > 1000)
  {
    lcd.writeChar(2);
  }
}

void buzzshort()
{
  buzzer.on();
  delay(1000);
  buzzer.off();
  delay(1000);
}

void buzzlong()
{
  buzzer.on();
  delay(2000);
  buzzer.off();
  delay(1000);
}

void loop()
{
  ppm = ens160.getECO2();
  temp = bme280.readTempC() - tempDiff;

  printCO2();
  printTemp();
  delay(200);

  if (ppm > 700)
  {
    buzzshort();
  }

  if (ppm > 1000)
  {
    buzzlong();
  }

  long currentMillis = millis();

  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    totalSeconds++;

    int hours = (totalSeconds / 3600) % 24;
    int minutes = (totalSeconds / 60) % 60;
    int seconds = totalSeconds % 60;

    Serial.printf("%02d:%02d:%02d\n", hours, minutes, seconds);
  }
}
