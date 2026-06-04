#include "display.h"
#include "lcd_driver.h"
#include "screen_buffer.h"

#include "bitmaps/background.h"

#include "bitmaps/banner/evasion_banner.h"
#include "bitmaps/banner/alert_banner.h"
#define BANNER_X         0
#define BANNER_Y         3
#define BANNER_WIDTH     160
#define BANNER_HEIGHT    24

#include "bitmaps/kanji/evasion_kanji.h"
#include "bitmaps/kanji/alert_kanji.h"
#define KANJI_X         20
#define KANJI_Y         34
#define KANJI_WIDTH     64
#define KANJI_HEIGHT    30

#include "bitmaps/meridiem/ante.h"
#include "bitmaps/meridiem/post.h"
#define MERIDIEM_X         95
#define MERIDIEM_Y         50
#define MERIDIEM_WIDTH     24
#define MERIDIEM_HEIGHT    14

#include "bitmaps/alarm_indicator.h"
#define ALARM_INDICATOR_X         95
#define ALARM_INDICATOR_Y         35
#define ALARM_INDICATOR_WIDTH     40
#define ALARM_INDICATOR_HEIGHT    11

#include "bitmaps/time/blank.h"
#include "bitmaps/time/zero.h"
#include "bitmaps/time/one.h"
#include "bitmaps/time/two.h"
#include "bitmaps/time/three.h"
#include "bitmaps/time/four.h"
#include "bitmaps/time/five.h"
#include "bitmaps/time/six.h"
#include "bitmaps/time/seven.h"
#include "bitmaps/time/eight.h"
#include "bitmaps/time/nine.h"
#define DIGIT_Y                   78
static const uint8_t DIGIT_X[4] = {20, 48, 89, 117};
#define DIGIT_WIDTH_BITS          24
#define DIGIT_HEIGHT_BITS         41
static const uint8_t* const DIGIT_BITMAPS[10] = {
  zero, one, two, three, four,
  five, six, seven, eight, nine
};

//================================================================================
static ScreenBuffer screenBuffer;

void setModeEvasion() 
{
  screenBuffer.overlayBitmap(evasion_banner, BANNER_X, BANNER_Y, BANNER_WIDTH, BANNER_HEIGHT);
  screenBuffer.overlayBitmap(evasion_kanji, KANJI_X, KANJI_Y, KANJI_WIDTH, KANJI_HEIGHT);
}

void setModeAlert() 
{
  screenBuffer.overlayBitmap(alert_banner, BANNER_X, BANNER_Y, BANNER_WIDTH, BANNER_HEIGHT);
  screenBuffer.overlayBitmap(alert_kanji, KANJI_X, KANJI_Y, KANJI_WIDTH, KANJI_HEIGHT);
}

void setDefaultDisplay()
{
  screenBuffer.setBuffer(background);
  setModeEvasion();
  displayBitmap(screenBuffer.data());

  // setModeAlert();
  // screenBuffer.overlayBitmap(alarm_indicator, ALARM_INDICATOR_X, ALARM_INDICATOR_Y, ALARM_INDICATOR_WIDTH, ALARM_INDICATOR_HEIGHT);
}

// Modify the buffer and display it given a Time object
void drawTime(const Time& now)
{
  uint8_t hour = now.hour;     // 1-12
  uint8_t minute = now.minute; // 0-59

  uint8_t h1 = hour / 10;
  uint8_t h2 = hour % 10;
  uint8_t m1 = minute / 10;
  uint8_t m2 = minute % 10;

  if (hour >= 10) {
    screenBuffer.overlayBitmap(DIGIT_BITMAPS[h1], DIGIT_X[0], DIGIT_Y, DIGIT_WIDTH_BITS, DIGIT_HEIGHT_BITS);
  } else {
    screenBuffer.overlayBitmap(blank, DIGIT_X[0], DIGIT_Y, DIGIT_WIDTH_BITS, DIGIT_HEIGHT_BITS);
  }
  screenBuffer.overlayBitmap(DIGIT_BITMAPS[h2], DIGIT_X[1], DIGIT_Y, DIGIT_WIDTH_BITS, DIGIT_HEIGHT_BITS);

  if (minute >= 10) {
    screenBuffer.overlayBitmap(DIGIT_BITMAPS[m1], DIGIT_X[2], DIGIT_Y, DIGIT_WIDTH_BITS, DIGIT_HEIGHT_BITS);
  } else {
    screenBuffer.overlayBitmap(DIGIT_BITMAPS[0], DIGIT_X[2], DIGIT_Y, DIGIT_WIDTH_BITS, DIGIT_HEIGHT_BITS);
  }
  screenBuffer.overlayBitmap(DIGIT_BITMAPS[m2], DIGIT_X[3], DIGIT_Y, DIGIT_WIDTH_BITS, DIGIT_HEIGHT_BITS);

  if (now.isPM) {
    screenBuffer.overlayBitmap(post, MERIDIEM_X, MERIDIEM_Y, MERIDIEM_WIDTH, MERIDIEM_HEIGHT);
  } else {
    screenBuffer.overlayBitmap(ante, MERIDIEM_X, MERIDIEM_Y, MERIDIEM_WIDTH, MERIDIEM_HEIGHT);
  }

  displayBitmap(screenBuffer.data());
}