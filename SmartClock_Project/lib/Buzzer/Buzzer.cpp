#include <SerLCD.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>
#include <SparkFun_Qwiic_Button.h>
#include "LCD.h"
#include "Timer.h"
#include "Menu.h"

QwiicBuzzer buzzer;
extern SerLCD lcd;
extern QwiicButton button;

enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE,
  TIMER_STATE
};

extern int ppm;
extern float temp;
extern int highPPM;
extern int midPPM;
extern menuState currentState;
bool buzzerMuted = false;
bool buzzerBuzzing = true;

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
      buzzerMuted = false;
      currentState = AIR_QUALITY_STATE;
    }

    if (swipeDown() || swipeLeft() || swipeRight() || swipeUp())
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
  buzzer.configureBuzzer(1000, 1000, SFE_QWIIC_BUZZER_VOLUME_MIN);
  buzzer.on();

  if (swipeDown() || swipeLeft() || swipeRight() || swipeUp())
  {
    buzzer.off();
    timerHasStarted = false;
    buzzerBuzzing = false;
    buzzerMuted = true;
  }
}