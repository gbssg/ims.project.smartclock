#include <SparkFun_Qwiic_Button.h>
#include <LCD.h>
#include <SimpleSoftTimer.h>

using namespace HolisticSolutions;

extern bool buzzerBuzzing;
enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE,
  TIMER_STATE
};

extern bool buttonRelease;
extern SimpleSoftTimer displayTime;
extern QwiicButton button;
extern menuState currentState;

void buttonReleaseHandler()
{
  if (!button.isPressedQueueEmpty() && buttonRelease == true)
  {
    buttonRelease = !buttonRelease;
    button.popPressedQueue();
  }
}

void handleMenuChange(menuState newState)
{
  if (!button.isPressedQueueEmpty() && buttonRelease == false)
  {
    currentState = newState;
    buttonRelease = !buttonRelease;
    button.popPressedQueue();
  }
}

void changeMenu()
{  
   switch (currentState) {
    case CLOCK_STATE:
    {
      handleMenuChange(AIR_QUALITY_STATE);
      buttonReleaseHandler();

      printTimeAndDate();
      break;
    }

    case AIR_QUALITY_STATE:
    {
      handleMenuChange(CLOCK_STATE);
      buttonReleaseHandler();

      printTempAndCO2();
      break;
    }

    /*
    case TIMER_STATE:
    {
      handleMenuChange(CLOCK_STATE);
      buttonReleaseHandler();
      break;
    }
    */
   
   }
} 


