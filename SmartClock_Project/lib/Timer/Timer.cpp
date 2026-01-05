
#include <Arduino.h>
#include "LCD.h"
#include "Menu.h"
#include <SparkFun_Qwiic_Button.h>
#define JoyStick_Y_Pin A0
#define JoyStick_X_Pin A1

extern int readAxisY;
extern int readAxisX;
extern int upRight;
extern int downLeft;
extern int hour;
extern int minute;
extern int second;
extern int arrowPosition;


extern String hourStr;
extern String minuteStr;
extern String secondStr;

enum menuState
{
  CLOCK_STATE,
  AIR_QUALITY_STATE,
  TIMER_STATE
};

extern menuState currentState;


bool swipeRight()
{
    if (readAxisX > upRight)
    {
        Serial.println("Swiped Right");
        return true;
    }
    return false;
}

bool swipeLeft()
{
    if (readAxisX < downLeft)
    {
        Serial.println("Swiped Left");
        return true;
    }
    return false;
}

bool swipeUp()
{
    if (readAxisY < upRight) 
    {
        Serial.println("Swiped Up");
        return true;
    }
    return false;
}

bool swipeDown()
{
    if (readAxisY < downLeft) 
    {
        Serial.println("Swiped Down");
        return true;
    }
    return false;
}

void showTimer()
{
if (displayTime.isTimeout()) 
  {
    lcd.setCursor(0,0);
    lcd.print("Timer: ");
    lcd.print(hourStr);
    lcd.print(":");
    lcd.print(minuteStr);
    lcd.print(":");
    lcd.print(secondStr);
    lcd.print(" ");
    displayTime.restart();
  }
}

void showTimerInterval()
{
    if (displayTime.isTimeout())
    {
        lcd.setCursor(arrowPosition, 1);
        lcd.writeChar(4);
        displayTime.restart();
    }
}

void setTimerInterval()
{
    if (currentState == TIMER_STATE && swipeRight() && arrowPosition < 14)
    {
        arrowPosition += 1;
        lcd.clear();
    }
    else if (currentState == TIMER_STATE && swipeLeft() && arrowPosition > 7)
    {
        arrowPosition -= 1;
        lcd.clear();
    }
}