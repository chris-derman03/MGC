#pragma once
#include <stdint.h>
#include "display_config.h"

class ScreenBuffer {
private:
  uint8_t buffer[SCREEN_BYTES];

  bool canDrawBitmap(uint8_t x, uint8_t y, uint8_t widthBits, uint8_t heightBits);

public:
  uint8_t* data();
  const uint8_t* data() const;

  void setBuffer(const uint8_t* bitmap);
  void setPixel(uint8_t x, uint8_t y, bool pixelOn);
  void overlayBitmap(const uint8_t* bitmap, uint8_t x, uint8_t y, uint8_t widthBits, uint8_t heightBits);
};