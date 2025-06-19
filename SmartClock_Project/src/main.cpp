#include <Wire.h>
#include "SparkFun_ENS160.h"
#include "SparkFunBME280.h"
#include <SparkFun_Qwiic_OLED.h>
#include <SerLCD.h>

SparkFun_ENS160 ens160;
BME280 bme280;
SerLCD lcd;

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

  Wire1.begin();
  lcd.begin(Wire1);
  lcd.setAddress(0x72);
  lcd.clear();
  lcd.setContrast(50);
}

void loop()
{
  lcd.print("CO2 : ");
  lcd.print(ens160.getECO2());
  lcd.println(" ppm");
  lcd.print("Temp: ");
  lcd.print(bme280.readTempC(), 2);
  lcd.println(" C");
  Serial.println("");
  delay(200);
}
