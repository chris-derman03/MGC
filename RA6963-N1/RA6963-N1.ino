#include "lcd_driver.h"
#include "clock.h"
#include "bitmaps/test.h"

#include <avr/pgmspace.h>

//================================================================================
uint8_t screenBuffer[SCREEN_BYTES];

Time testTime = Time{2026,6,3,6,49,1};

//================================================================================
void setup()
{
  Serial.begin(9600);

  rtc_setup();
  lcd_setup();
  initialize_display();

  setTime(testTime);
}

void loop()
{
  memcpy_P(screenBuffer, test, SCREEN_BYTES);
  displayBuffer(screenBuffer);

  printTime();

  while (6 < 7) {
    // printTime();
    // delay(1000);
  }
}



