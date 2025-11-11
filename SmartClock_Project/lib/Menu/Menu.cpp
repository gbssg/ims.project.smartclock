#include <SparkFun_Qwiic_Button.h>
#include <LCD.h>
#include <SimpleSoftTimer.h>

using namespace HolisticSolutions;

extern bool buzzerBuzzing;
enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE
};

extern bool buttonRelease;
extern SimpleSoftTimer displayTime;
extern QwiicButton button;
extern menuState currentState;

void changeMenu()
{  
   switch (currentState) {
    case CLOCK_STATE:
    {
      if (!button.isPressedQueueEmpty() && buttonRelease == false)
      {
        Serial.println("Button down 1");
        currentState = AIR_QUALITY_STATE;
        buttonRelease = !buttonRelease;
        button.popPressedQueue();
      }
      else if (!button.isPressedQueueEmpty() && buttonRelease == true)
      {
        Serial.println("Button up 1");
        buttonRelease = !buttonRelease;
        button.popPressedQueue();
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
      if (!button.isPressedQueueEmpty() && buttonRelease == false)
      {
        Serial.println("Button down 2");
        currentState = CLOCK_STATE;
        buttonRelease = !buttonRelease;
        button.popPressedQueue();
      }
      else if (!button.isPressedQueueEmpty() && buttonRelease == true)
      {
        Serial.println("Button up 2");
        buttonRelease = !buttonRelease;
        button.popPressedQueue();
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