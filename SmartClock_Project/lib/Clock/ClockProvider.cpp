#include "ClockProvider.h"
#include <Arduino.h>
#include <ESP32Time.h>
#include <time.h>

/** \file
 *  \addtogroup Clockprovider time provider
 *  @{
 *  \brief  manages time functions
 *
 *          # Theory of operation
 *          Handels basic time returnig functions.
 *
 *
 *          # How to use the module
 *          Is used to get local time.
 *
 *
 *          # Inner workings
 *          This module provides the necessary time information using the rtc library.
 *
 *
 */

static ESP32Time rtc;

/**
 * \brief   Initializes the clock access module
 *
 * \param[in]   offset  [s] Number of seconds the timezone is away from UTC
 */
void CP_init(int offset)
{
    // Declares a variable from the library <time.h> (struct tm)
    struct tm build_time;

    // returns a date time string, (__DATE__ " " __TIME__) is going to be replaced by current date / time
    strptime(__DATE__ " " __TIME__, "%b %d %Y %H:%M:%S", &build_time); // Converter
    rtc.setTime(mktime(&build_time) - offset);

    rtc.offset = offset;
}

/**
 * \brief   Formats the current hour as string
 *
 * \return  String with the current hour as string formatted number
 */
String CP_getHourAsString()
{
    // Serial.println(rtc.getTime("%H"));s
    return rtc.getTime("%H");
}

/**
 * \brief   Formats the current minute as string
 *
 * \return  String with the current minute as string formatted number
 */
String CP_getMinuteAsString()
{
    // Serial.println(rtc.getTime("%M"));
    return rtc.getTime("%M");
}

String CP_getSecondAsString()
{
    // Serial.println(rtc.getTime("%M"));
    return rtc.getTime("%S");
}

String CP_getDayAsString()
{

    return rtc.getTime("%d");
}

String CP_getMonthAsString()
{

    return rtc.getTime("%b");
}

String CP_getYearAsString()
{

    return rtc.getTime("%Y");
}

/**
 * \brief Gets one Time Position as Integer
 *
 * \return Integer from given position in time string
 */
int CP_timeIntGet(int position)
{
    String timeString = rtc.getTime("%H%M"); // Possible Output: 2019
    int timeInt = timeString.charAt(position);
    return timeInt;
}

/** @} */
