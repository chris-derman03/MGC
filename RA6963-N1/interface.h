#pragma once
#include "clock.h"

void interface_setup();
bool handleButtons(
  bool& alarmEnabled,
  bool& alarmActive,
  bool& setTimeActive,
  bool& setAlarmTimeActive,
  bool& snoozeActive,
  Time& alarmTime,
  Time& editTime
);