
#include <Arduino.h>
#include "LCD.h"
#include "Menu.h"
#include <SparkFun_Qwiic_Button.h>
#include <Buzzer.h>
#include <SparkFun_Qwiic_Buzzer_Arduino_Library.h>
#define JoyStick_Y_Pin A0
#define JoyStick_X_Pin A1

extern int readAxisY;
extern int readAxisX;
int upRight = 75;
int downLeft = 25;
int arrowPosition = 14;
bool swipedUp;
bool timerHasStarted = false;

int hour;
int minute;
int second;

String hourStr = hour < 10 ? "0" + String(hour) : String(hour);
String minuteStr = minute < 10 ? "0" + String(minute) : String(minute);
String secondStr = second < 10 ? "0" + String(second) : String(second);

SimpleSoftTimer arrowPositionTimer(250);
SimpleSoftTimer valueChangeTimer(500);
SimpleSoftTimer timerCountdown(1000);

enum menuState
{
    CLOCK_STATE,
    AIR_QUALITY_STATE,
    TIMER_STATE
};

extern menuState currentState;

void timerInit()
{
    arrowPositionTimer.start(250);
    valueChangeTimer.start(500);
    timerCountdown.start(1000);
}
void initializeTimerVariables()
{
    hourStr = hour < 10 ? "0" + String(hour) : String(hour);
    minuteStr = minute < 10 ? "0" + String(minute) : String(minute);
    secondStr = second < 10 ? "0" + String(second) : String(second);
}

bool swipeRight()
{
    if (readAxisX > upRight && valueChangeTimer.isTimeout())
    {
        Serial.println("Swiped Right");
        valueChangeTimer.restart();
        return true;
    }
    return false;
}

bool swipeLeft()
{
    if (readAxisX < downLeft && valueChangeTimer.isTimeout())
    {
        Serial.println("Swiped Left");
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

bool swipeUp()
{
    if (readAxisY > upRight && valueChangeTimer.isTimeout())
    {
        Serial.println("Swiped Up");
        swipedUp = true;
        valueChangeTimer.restart();
        return true;
    }
    else if (swipedUp && swipeDown() || swipeLeft() || swipeRight())
    {
        swipedUp = false;
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
        if (arrowPosition >= 7)
        {
            lcd.writeChar(4);
        }
        lcd.setCursor(2, 1);
        lcd.writeChar(5);
        displayTimer.restart();
    }
}

void chooseOption()
{

    bool leftDetected = false;
    bool rightDetected = false;
    if (arrowPositionTimer.isTimeout())
    {
        rightDetected = swipeRight();
        leftDetected = swipeLeft();

        if (rightDetected && arrowPosition < 14 && arrowPosition >= 7)
        {
            do
            {
                arrowPosition += 1;
            } while ((arrowPosition == 9 || arrowPosition == 12) && arrowPosition < 14);
            lcd.clear();
            Serial.println(arrowPosition);
        }
        else if (leftDetected && arrowPosition > 7)
        {
            do
            {
                arrowPosition -= 1;
            } while ((arrowPosition == 9 || arrowPosition == 12));
            lcd.clear();
            Serial.println(arrowPosition);
        }
        arrowPositionTimer.restart();
    }

    if (arrowPosition == 7 && leftDetected)
    {
        arrowPosition = 1;
        lcd.clear();
        lcd.setCursor(2, 1);
    }
    else if (arrowPosition < 7 && rightDetected)
    {
        arrowPosition = 7;
        lcd.clear();
    }
}

void setTimer()
{
    switch (arrowPosition)
    {
    case 7:
        if (swipeUp())
        {
            hour += 10;
            if (hour > 99)
                hour = 99;
        }
        else if (swipeDown())
        {
            hour -= 10;
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
            minute += 10;
            if (minute > 59)
                minute = 59;
        }
        else if (swipeDown())
        {
            minute -= 10;
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
            second += 10;
            if (second > 59)
                second = 59;
        }
        else if (swipeDown())
        {
            second -= 10;
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

void startTimer()
{
    if (timerCountdown.isTimeout())
    {

        if (arrowPosition == 1 && swipedUp && (hour > 0 || minute > 0 || second > 0))
        {
            timerHasStarted = true;
            lcd.setCursor(2, 1);
            lcd.writeChar(6);

            if (second > 0)
            {
                second--;
            }
            else if (minute > 0)
            {
                minute--;
                second = 59;
            }
            else if (hour > 0)
            {
                hour--;
                minute = 59;
                second = 59;
            }
        }
        else if (arrowPosition == 1 && swipedUp && hour == 0 && minute == 0 && second == 0 && timerHasStarted)
        {
            button.clearEventBits();
            buzzerBuzzing = true;
            timerBuzz();
            currentState = TIMER_STATE;
        }
        timerCountdown.restart();
    }
}