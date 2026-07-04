#include "lcd_driver.h"
#include "clock.h"
#include "display.h"
#include "interface.h"

#define TIME_CHECK_INTERVAL    100    //ms
#define BUTTON_CHECK_INTERVAL  5      //ms
#define SNOOZE_DURATION        300000 // 5 min

//================================================================================
Time defaultTime = Time{2026,6,3,12,0,false};
Time alarmTime = Time{0,0,0,12,1,false};

//================================================================================
void setup()
{
  Serial.begin(9600);
  rtc_setup();
  lcd_setup();
  interface_setup();
  initialize_display();
  setDefaultDisplay();

  setTime(defaultTime);
  drawTime(getTime());
}

bool timesMatch(const Time& now, const Time& alarmTime)
{
  return (
    now.hour == alarmTime.hour &&
    now.minute == alarmTime.minute &&
    now.isPM == alarmTime.isPM
  );
}

void loop()
{
  static bool alarmEnabled = false;
  static bool alarmActive = false;
  static bool snoozeActive = false;
  static uint32_t snoozeStart = 0;
  static Time editTime;
  static bool setTimeActive = false;
  static bool setAlarmTimeActive = false;

  static bool displayNeedsUpdate = false;

  // ================================================================
  // INTERFACE
  // ================================================================
  static uint32_t lastButtonCheck = 0;

  // Runs every BUTTON_CHECK_INTERVAL ms
  if (millis() - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    lastButtonCheck = millis();

    displayNeedsUpdate |= handleButtons(
      alarmEnabled,
      alarmActive,
      setTimeActive,
      setAlarmTimeActive,
      snoozeActive,
      alarmTime,
      editTime
    );
  }

  // ================================================================
  // TIME DISPLAY
  // ================================================================
  static uint32_t lastRtcCheck = 0;
  static int currentMinute = -1;

  static bool lastSetAlarmTimeActive = false;
  static bool lastSetTimeActive = false;

  bool timeSetChanged = setTimeActive != lastSetTimeActive;
  bool alarmSetChanged = setAlarmTimeActive != lastSetAlarmTimeActive;

  // redraw immediately if mode changes
  if (timeSetChanged || alarmSetChanged) {

    if (timeSetChanged) {
      if (setTimeActive) { // changed false -> true
        editTime = getTime();
        drawTime(editTime);
      } 
      else { // changed true -> false
        setTime(editTime);
        Time now = getTime();
        currentMinute = now.minute;
        drawTime(now);
      }
    }

    if (alarmSetChanged) {
      if (setAlarmTimeActive) { // changed false -> true
        drawTime(alarmTime);
      } else if (!setTimeActive) { // changed true -> false
        Time now = getTime();
        currentMinute = now.minute;
        drawTime(now);
      }
    }

    lastSetTimeActive = setTimeActive;
    lastSetAlarmTimeActive = setAlarmTimeActive;
  }

  // redraw changes in alarm edit and time edit modes
  if (displayNeedsUpdate) {
    if (setAlarmTimeActive) {
      drawTime(alarmTime);
    } 
    else if (setTimeActive) {
      drawTime(editTime);
    }
    displayNeedsUpdate = false;
  }

  // check if time changes every TIME_CHECK_INTERVAL
  if (!setAlarmTimeActive && !setTimeActive &&
    millis() - lastRtcCheck >= TIME_CHECK_INTERVAL) {

    lastRtcCheck = millis();

    Time now = getTime();

    // Set off alarm
    if (alarmEnabled && !alarmActive && !snoozeActive && timesMatch(now, alarmTime)) {
      alarmActive = true;
      setModeAlert();
    }

    // redraw time if needed
    if (now.minute != currentMinute) {
      currentMinute = now.minute;
      drawTime(now);
    }
  }

  // ================================================================
  // misc
  // ================================================================
  static bool lastAlarmToggleState = false;
  bool alarmToggleChanged = alarmEnabled != lastAlarmToggleState;

  if (alarmToggleChanged) {
    if (alarmEnabled) {
      showAlarmIndicator();
    } else {
      hideAlarmIndicator();
    }
    lastAlarmToggleState = alarmEnabled;
  }

  // Snoozed while alarm is active
  if (snoozeActive && alarmActive) {
    alarmActive = false;
    snoozeStart = millis();
    setModeEvasion();
  }

  // If alarm turned off clear everything
  if (!alarmEnabled) {
    if (alarmActive || snoozeActive) {
      alarmActive = false;
      snoozeActive = false;
      setModeEvasion();
    }
  }

  // Reactivate alarm after snooze period
  if (snoozeActive && !alarmActive) {
    if (millis() - snoozeStart >= SNOOZE_DURATION) {
      alarmActive = true;
      snoozeActive = false;
      setModeAlert();
    }
  }
}



