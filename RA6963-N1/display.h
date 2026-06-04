#pragma once
#include "clock.h"

void setDefaultDisplay();
void showAlarmIndicator();
void hideAlarmIndicator();
void setModeEvasion();
void setModeAlert();
void drawTime(const Time& now);