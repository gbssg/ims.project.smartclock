#include <SerLCD.h>
#include <SimpleSoftTimer.h>

using namespace HolisticSolutions;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern SerLCD lcd;
extern SimpleSoftTimer displayTime;

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


