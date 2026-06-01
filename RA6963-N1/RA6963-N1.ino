#include "lcd_driver.h"
#include "bitmaps/test.h"

//================================================================================
uint8_t screenBuffer[SCREEN_BYTES];


//================================================================================
void setup()
{
  lcd_setup();
  initialize_display();
  clearDisplay();
}

void loop()
{
  memcpy_P(screenBuffer, test, SCREEN_BYTES);
  displayBuffer(screenBuffer);

  while (6 < 7);
}



