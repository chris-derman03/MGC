#include <Arduino.h>
#include "clock.h"
#include "RTClib.h"
#include <Wire.h>

static RTC_DS3231 rtc;

void rtc_setup() 
{
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Error: Couldn't find RTC");
    while(1);
  }
}

//==============================================================================
// Fake time for testing display with faster time
// Use Markiplier (multiplier) to set fake time rate
//==============================================================================
#define DEBUG               false
#define MARKIPLIER          30
#define DEBUG_START_HOUR    0
#define DEBUG_START_MINUTE  0
#define DEBUG_START_SECOND  0

static DateTime getMarkipliedTime() {
  uint32_t startSeconds = DEBUG_START_HOUR * 3600UL + DEBUG_START_MINUTE * 60UL + DEBUG_START_SECOND;
  uint32_t markipliedUpSeconds = ((millis() / 1000UL) * MARKIPLIER);
  uint32_t markipliedSeconds = (markipliedUpSeconds + startSeconds) % 86400UL;

  uint8_t hour24 = markipliedSeconds / 3600UL;
  uint8_t minute = (markipliedSeconds % 3600UL) / 60UL;
  uint8_t second = markipliedSeconds % 60UL;

  return DateTime(2026, 6, 7, hour24, minute, second);
}

//==============================================================================
// RTC Interface
//==============================================================================

// Set the RTC
void setTime(const Time &now) 
{
  uint8_t hour = (now.hour % 12) + 12 * (now.isPM);

  // DateTime() is Year, Month, Day, Hour, Minute, Second
  rtc.adjust(DateTime(now.year, now.month, now.day, hour, now.minute, 0));
}

// Probe the RTC
Time getTime() 
{
  DateTime now = DEBUG ? getMarkipliedTime() : rtc.now();

  return Time{now.year(), now.month(), now.day(), now.twelveHour(), now.minute(), now.isPM()};
}

// DEBUG print
void printTime() 
{
  DateTime now = DEBUG ? getMarkipliedTime() : rtc.now();

  String time = String(now.twelveHour()) + ":";

  if (now.minute() < 10) {
    time += "0";
  }

  time += String(now.minute()) + " ";
  time += (now.isPM() ? "PM" : "AM");

  Serial.println(time);
}

//==============================================================================
// Time Struct Interface
//==============================================================================
void incrementHour(Time& t)
{
  if (t.hour == 11) {
    t.hour = 12;
    t.isPM = !t.isPM;   // 11 AM -> 12 PM, 11 PM -> 12 AM
  } else if (t.hour == 12) {
    t.hour = 1;
  } else {
    t.hour++;
  }
}

void incrementMinute(Time& t)
{
  t.minute++;

  if (t.minute > 59) {
    t.minute = 0;
    incrementHour(t);
  }
}


