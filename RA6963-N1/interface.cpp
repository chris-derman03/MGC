#include <Arduino.h>
#include "interface.h"

#define TIME_SET_PIN      30
#define HOUR_PIN          32
#define MINUTE_PIN        34
#define ALARM_SET_PIN     36
#define ALARM_ENABLE_PIN  38
#define SNOOZE_PIN        40

void interface_setup()
{
  pinMode(TIME_SET_PIN, INPUT_PULLUP);
  pinMode(HOUR_PIN, INPUT_PULLUP);
  pinMode(MINUTE_PIN, INPUT_PULLUP);
  pinMode(ALARM_SET_PIN, INPUT_PULLUP);
  pinMode(ALARM_ENABLE_PIN, INPUT_PULLUP);
  pinMode(SNOOZE_PIN, INPUT_PULLUP);
}

static bool pressed(uint8_t pin)
{
  return digitalRead(pin) == LOW;
}

static bool justPressed(uint8_t pin)
{
  static bool lastState[54];

  bool now = pressed(pin);
  bool result = now && !lastState[pin];

  lastState[pin] = now;
  return result;
}

static bool repeatPressed(uint8_t pin)
{
  static bool lastPressed[54] = {false};
  static uint32_t pressStart[54] = {0};
  static uint32_t lastRepeat[54] = {0};

  const uint32_t HOLD_DELAY = 1000;      // wait 1 sec before rapid repeat
  const uint32_t REPEAT_INTERVAL = 100;  // then repeat every 100ms

  bool nowPressed = pressed(pin);
  uint32_t now = millis();

  // just pressed: fire once immediately
  if (nowPressed && !lastPressed[pin]) {
    pressStart[pin] = now;
    lastRepeat[pin] = now;
    lastPressed[pin] = true;
    return true;
  }

  // held: after 1 sec, repeat
  if (nowPressed && lastPressed[pin]) {
    if (now - pressStart[pin] >= HOLD_DELAY &&
        now - lastRepeat[pin] >= REPEAT_INTERVAL) {
      lastRepeat[pin] = now;
      return true;
    }
  }

  // released
  if (!nowPressed) {
    lastPressed[pin] = false;
  }

  return false;
}

bool handleButtons(
      bool& alarmEnabled,
      bool& alarmActive,
      bool& setTimeActive,
      bool& setAlarmTimeActive,
      bool& snoozeActive,
      Time& alarmTime,
      Time& editTime
    )
{
  bool displayNeedsUpdate = false;

  bool timeSetHeld = pressed(TIME_SET_PIN);
  bool alarmSetHeld = pressed(ALARM_SET_PIN);
  bool alarmToggled = pressed(ALARM_ENABLE_PIN);

  setTimeActive = timeSetHeld;
  setAlarmTimeActive = alarmSetHeld;
  alarmEnabled = alarmToggled;

  if (timeSetHeld) {
    if (repeatPressed(HOUR_PIN)) {
      incrementHour(editTime);
      displayNeedsUpdate = true;
    }

    if (repeatPressed(MINUTE_PIN)) {
      incrementMinute(editTime);
      displayNeedsUpdate = true;
    }
  }
  else if (alarmSetHeld) {
    if (repeatPressed(HOUR_PIN)) {
      incrementHour(alarmTime);
      displayNeedsUpdate = true;
    }

    if (repeatPressed(MINUTE_PIN)) {
      incrementMinute(alarmTime);
      displayNeedsUpdate = true;
    }
  }

  if (justPressed(SNOOZE_PIN) && alarmActive) {
    snoozeActive = true;
  }

  return displayNeedsUpdate;
}