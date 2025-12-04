#include <SparkFun_Qwiic_Button.h>
#include <LCD.h>
#include <SimpleSoftTimer.h>
# 

using namespace HolisticSolutions;

enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE,
  TIMER_STATE
};

extern bool buzzerBuzzing;

extern bool buttonRelease;
extern SimpleSoftTimer displayTime;
extern QwiicButton button;
extern menuState currentState;

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

// Hauptfunktion zum Verwalten des Menüs
void manageMenu()
{  
   switch (currentState) {
    case CLOCK_STATE:
    {
      handleMenuChange(AIR_QUALITY_STATE);
      buttonReleaseHandler();

      printTimeAndDate();
      Serial.println("Time and Date Menu");
      break;
    }

    case AIR_QUALITY_STATE:
    {
      handleMenuChange(TIMER_STATE);
      buttonReleaseHandler();

      printTempAndCO2();
      Serial.println("Air Quality Menu");
      break;
    }
    
    case TIMER_STATE:
    {
      if (displayTime.isTimeout()) 
        {
          lcd.clear();
        }
      handleMenuChange(CLOCK_STATE);
      buttonReleaseHandler();

      readAxes();
      Serial.println("Timer Menu (Read Axes)");
    }
   }
} 


