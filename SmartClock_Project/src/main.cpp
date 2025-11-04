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

#define UTC_OFFSET_SECONDS 3600 // UTC+1
using namespace HolisticSolutions;

// Variabeln definieren
int ppm;
int tempDiff = 5;
float temp;
int brightness = 100;
int highPPM = 1000;
int midPPM = 800;
int highTemp = 28;
bool buzzerMuted = false;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_SECONDS);

// Objekte erstellen
QwiicBuzzer buzzer;
SparkFun_ENS160 ens160;
BME280 bme280;
SerLCD lcd;
QwiicButton button;
SimpleSoftTimer displayTime(1000);

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

// Funktion für die Anzeige der Temperatur

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
  // printTemp
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp, 2);
  lcd.println(" C   ");

  // printCO2
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

// Funktion für das Warnsignal des Buzzers
void warnBuzz()
{
  if(ppm > midPPM)
  {
    if(!buzzerMuted)
    {
      button.LEDoff();
      buzzer.configureBuzzer(2730, 1000, SFE_QWIIC_BUZZER_VOLUME_MIN);
      buzzer.on();
      printTempAndCO2();
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



// Setup Funktion
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  displayTime.start(1000);
  connectWifi();
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

// Loop Funktion
void loop()
{
  timeClient.begin();
  timeClient.update();


  // CO2 Wert und Temperatur auslesen
  ppm = ens160.getECO2();
  temp = bme280.readTempC() - tempDiff;
  printTempAndCO2();

  // Buzzer Funktion aufrufen
  warnBuzz();

  // LED vom Button steuern
  if (button.isPressed())
  {
    button.LEDon(brightness);
  }
  else
  {
    button.LEDoff(); 
  }

  if (displayTime.isTimeout()) {
  displayTime.restart();
 }

 
}
