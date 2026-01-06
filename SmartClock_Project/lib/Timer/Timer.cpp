
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

extern SimpleSoftTimer arrowPositionTimer;
extern SimpleSoftTimer valueChangeTimer;
enum menuState
{
    CLOCK_STATE,
    AIR_QUALITY_STATE,
    TIMER_STATE
};

extern menuState currentState;

void initializeTimerVariables()
{
    hourStr = hour < 10 ? "0" + String(hour) : String(hour);
    minuteStr = minute < 10 ? "0" + String(minute) : String(minute);
    secondStr = second < 10 ? "0" + String(second) : String(second);
}

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
    if (readAxisY > upRight && valueChangeTimer.isTimeout())
    {
        Serial.println("Swiped Up");
        valueChangeTimer.restart();
        return true;
    }
    return false;
}

bool swipeDown()
{
    if (readAxisY < downLeft && valueChangeTimer.isTimeout())
    {
        Serial.println("Swiped Down");
        valueChangeTimer.restart();
        return true;
    }
    return false;
}

void showTimer()
{
    if (displayTimer.isTimeout())
    {
        lcd.setCursor(0, 0);
        lcd.print("Timer: ");
        lcd.print(hourStr);
        lcd.print(":");
        lcd.print(minuteStr);
        lcd.print(":");
        lcd.print(secondStr);
        lcd.print(" ");
        lcd.setCursor(arrowPosition, 1);
        lcd.writeChar(4);
        displayTimer.restart();
    }
}

void setTimerInterval()
{
    if (arrowPositionTimer.isTimeout())
    {
        if (swipeRight() && arrowPosition < 14)
        {
            do
            {
                arrowPosition += 1;
            } while ((arrowPosition == 9 || arrowPosition == 12) && arrowPosition < 14);
            lcd.clear();
            Serial.println(arrowPosition);
        }
        else if (swipeLeft() && arrowPosition > 7)
        {
            do
            {
                arrowPosition -= 1;
            } while ((arrowPosition == 9 || arrowPosition == 12) && arrowPosition > 7);
            lcd.clear();
            Serial.println(arrowPosition);
        }
        arrowPositionTimer.restart();
    }
}

void setTimer()
{
    switch (arrowPosition)
    {
    case 7:
        if (swipeUp())
        {
            hour += 5;
            if (hour > 99)
                hour = 99;
        }
        else if (swipeDown())
        {
            hour -= 5;
            if (hour < 0)
                hour = 0;
        }

        break;
    case 8:
        if (swipeUp())
        {
            hour += 1;
            if (hour > 99)
                hour = 99;
        }
        else if (swipeDown())
        {
            hour -= 1;
            if (hour < 0)
                hour = 0;
        }
        break;
    case 10:
        if (swipeUp())
        {
            minute += 15;
            if (minute > 59)
                minute = 59;
        }
        else if (swipeDown())
        {
            minute -= 15;
            if (minute < 0)
                minute = 0;
        }
        break;
    case 11:
        if (swipeUp())
        {
            minute += 1;
            if (minute > 59)
                minute = 59;
        }
        else if (swipeDown())
        {
            minute -= 1;
            if (minute < 0)
                minute = 0;
        }
        break;
    case 13:
        if (swipeUp())
        {
            second += 15;
            if (second > 59)
                second = 59;
        }
        else if (swipeDown())
        {
            second -= 15;
            if (second < 0)
                second = 0;
        }
        break;
    case 14:
        if (swipeUp())
        {
            second += 1;
            if (second > 59)
                second = 59;
        }
        else if (swipeDown())
        {
            second -= 1;
            if (second < 0)
                second = 0;
        }
        break;
    }
    initializeTimerVariables();
}
