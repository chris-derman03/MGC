#include "lcd_driver.h"
#include "clock.h"
#include "display.h"

#define TIME_CHECK_INTERVAL    1000 //ms
#define BUTTON_CHECK_INTERVAL    5 //ms

//================================================================================
Time testTime = Time{2026,6,3,8,56,1};

//================================================================================
void setup()
{
  Serial.begin(9600);
  rtc_setup();
  lcd_setup();
  initialize_display();
  setDefaultDisplay();

  setTime(testTime);
}

void loop()
{
  static uint32_t lastButtonCheck = 0;

  // // Runs every BUTTON_CHECK_INTERVAL ms
  // if (millis() - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
  //   lastButtonCheck = millis();
  //   handleButtons();
  // }

  static uint32_t lastRtcCheck = 0;
  static int currentMinute = -1;

  // Runs every TIME_CHECK_INTERVAL ms
  if (millis() - lastRtcCheck >= TIME_CHECK_INTERVAL) {
    lastRtcCheck = millis();

    Time now = getTime();

    if(now.minute != currentMinute) {
      currentMinute = now.minute;
      drawTime(now);
      printTime();
    }
  }
}



