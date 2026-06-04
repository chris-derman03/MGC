#pragma once
#include <stdint.h>

struct Time {
  uint16_t year;
  uint8_t  month;
  uint8_t  day;
  uint8_t  hour; // 1-12
  uint8_t  minute;
  bool     isPM;
};

void rtc_setup();

void setTime(const Time &now);
Time getTime();
void printTime();
void incrementHour(Time& t);
void incrementMinute(Time& t);