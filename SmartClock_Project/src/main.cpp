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
bool buzzerBuzzing = true;
enum menuState 
{
  CLOCK_STATE,
  AIR_QUALITY_STATE
};
menuState currentState;

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

  // V2
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
}

// Funktion für das Warnsignal des Buzzers
void warnBuzz()
{
  if(ppm > midPPM)
  {
    if(!buzzerMuted)
    {
      buzzerBuzzing = true;
      button.LEDoff();
      buzzer.configureBuzzer(2730, 1000, SFE_QWIIC_BUZZER_VOLUME_MIN);
      buzzer.on();
      printTempAndCO2();
    }
    
    if (button.isPressed())
    {
      buzzer.off();
      buzzerBuzzing = false;
      buzzerMuted = true;
    }
  }
  else if (ppm < (midPPM - 100))
  {
    buzzerBuzzing = false;
    buzzerMuted = false;
  }
}

void changeMenu()
{  
   switch (currentState) {
    case CLOCK_STATE:
    {
      if (button.hasBeenClicked())
      {
        currentState = AIR_QUALITY_STATE;
        button.clearEventBits();
      }

      if (displayTime.isTimeout()) 
      {
        printTimeAndDate();
        displayTime.restart();
      }
      
      break;
    }
    case AIR_QUALITY_STATE:
    {
      if (button.hasBeenClicked() && buzzerBuzzing == false)
      {
        currentState = CLOCK_STATE;
        button.clearEventBits();
      }

      if (displayTime.isTimeout()) 
      {    
        printTempAndCO2();
        displayTime.restart();
      }

      break;
    }
   }
} 


// Setup Funktion
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  displayTime.start(1000);
  connectWifi();
  timeClient.begin();

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
  timeClient.update();
  // CO2 Wert und Temperatur auslesen
  ppm = ens160.getECO2();
  temp = bme280.readTempC() - tempDiff;

  // Menü-Funktion aufrufen
  changeMenu();
  
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

}
