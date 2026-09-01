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

menuState currentState = CLOCK_STATE;
menuState lastState = CLOCK_STATE;

extern bool buzzerBuzzing;

bool buttonRelease = false;
extern SimpleSoftTimer displayTimer;
extern QwiicButton button;
extern SimpleSoftTimer changeMenuTimer;

// Change Menu (30s)
void changeMenuAutomatically()
{

  if (!changeMenuTimer.isTimeout())
    return;

  // Rotate menu: CLOCK -> AIR_QUALITY -> (TIMER if started) -> CLOCK
  switch (currentState)
  {
  case CLOCK_STATE:
    currentState = AIR_QUALITY_STATE;
    break;
  case AIR_QUALITY_STATE:
    // timerHasStarted is expected to be defined elsewhere
    if (timerHasStarted)
      currentState = TIMER_STATE;
    else
      currentState = CLOCK_STATE;
    break;
  case TIMER_STATE:
    currentState = CLOCK_STATE;
    break;
  }

  // restart automatic rotation timer
  changeMenuTimer.start(30000);
}

// Funktion zum Ändern des Menüzustands bei Tastendruck
void handleMenuChange(menuState newState)
{
  if (!button.isPressedQueueEmpty() && buttonRelease == false && !buzzerBuzzing)
  {
    currentState = newState;
    buttonRelease = !buttonRelease;
    button.popPressedQueue();
    changeMenuTimer.start(30000);
  }
}

// Funktion um beim Klick, buttonRelease zu umkehren
void buttonReleaseHandler()
{
  if (!button.isPressedQueueEmpty() && buttonRelease == true && !buzzerBuzzing)
  {
    buttonRelease = !buttonRelease;
    button.popPressedQueue();
  }
}

// Funktiom um die Anzeige einmal pro wechseln zu reinigen
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
  changeMenuAutomatically();

  switch (currentState)
  {
  case CLOCK_STATE:
  {
    handleMenuChange(AIR_QUALITY_STATE);
    buttonReleaseHandler();

    clearDisplayOnce();
    printTimeAndDate();
    break;
  }

  case AIR_QUALITY_STATE:
  {
    handleMenuChange(TIMER_STATE);
    buttonReleaseHandler();

    clearDisplayOnce();
    printTempAndCO2();
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
    break;
  }
  }
}
