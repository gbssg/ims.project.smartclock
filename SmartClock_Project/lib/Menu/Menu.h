#pragma once

#include <SparkFun_Qwiic_Button.h>
#include <LCD.h>
#include <SimpleSoftTimer.h>

using namespace HolisticSolutions;

void manageMenu();

void handleMenuChange();

void buttonReleaseHandler();