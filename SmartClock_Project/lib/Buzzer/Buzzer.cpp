#include <SerLCD.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>
#include <SparkFun_Qwiic_Button.h>
#include "LCD.h"
#include "Timer.h"

extern QwiicBuzzer buzzer;
extern SerLCD lcd;
extern QwiicButton button;

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern bool buzzerMuted;
extern bool buzzerBuzzing;

// Funktion für das Warnsignal des Buzzers
void warnBuzz()
{
  if (ppm > midPPM)
  {
    if (!buzzerMuted)
    {
      buzzerBuzzing = true;
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

void timerBuzz()
{
  buzzerBuzzing = true;
  buzzer.configureBuzzer(2730, 1000, SFE_QWIIC_BUZZER_VOLUME_MIN);
  buzzer.on();

  if (button.hasBeenClicked() || swipeDown() || swipeLeft() || swipeRight() || swipeUp())
  {
    buzzer.off();
    timerHasStarted = false;
    buzzerBuzzing = false;
    buzzerMuted = true;
    button.clearEventBits();
  }
}