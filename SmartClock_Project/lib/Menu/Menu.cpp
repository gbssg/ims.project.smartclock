#include <SparkFun_Qwiic_Button.h>
#include <LCD.h>
#include <SimpleSoftTimer.h>
#include <Timer.h>

using namespace HolisticSolutions;

enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE,
  TIMER_STATE
};

extern bool buzzerBuzzing;

extern bool buttonRelease;
extern SimpleSoftTimer displayTimer;
extern QwiicButton button;
extern menuState currentState;
extern menuState lastState;

// Funktion zum Verwalten des Menüs basierend auf dem aktuellen Zustand
void buttonReleaseHandler()
{
  if (!button.isPressedQueueEmpty() && buttonRelease == true)
  {
    buttonRelease = !buttonRelease;
    button.popPressedQueue();
  }
}

// Funktion zum Ändern des Menüzustands bei Tastendruck
void handleMenuChange(menuState newState)
{
  if (!button.isPressedQueueEmpty() && buttonRelease == false)
  {
    currentState = newState;
    buttonRelease = !buttonRelease;
    button.popPressedQueue();
  }
}

// Funktiom um die Anzeige, einmal pro wechseln zu reinigen
void clearDisplayOnce()
{
  if (lastState != currentState)
  {
    lcd.clear();
    Serial.println("LCD Cleared once");
    lastState = currentState;
  }
}

// Hauptfunktion zum Verwalten des Menüs
void manageMenu()
{
  switch (currentState)
  {
  case CLOCK_STATE:
  {
    handleMenuChange(AIR_QUALITY_STATE);
    buttonReleaseHandler();

    clearDisplayOnce();
    printTimeAndDate();
    // Serial.println("Clock_State");
    break;
  }

  case AIR_QUALITY_STATE:
  {
    handleMenuChange(TIMER_STATE);
    buttonReleaseHandler();

    clearDisplayOnce();
    printTempAndCO2();
    // Serial.println("Air_Quality_State");
    break;
  }
  case TIMER_STATE:
  {
    handleMenuChange(CLOCK_STATE);
    buttonReleaseHandler();

    clearDisplayOnce();
    showTimer();
    chooseOption();
    setTimer();
    swipeUp();
    // Serial.println("Timer_State");
    break;
  }
  }
}
