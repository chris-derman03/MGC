#include <Arduino.h>
#include <avr/pgmspace.h>
#include "screen_buffer.h"

uint8_t* ScreenBuffer::data()
{
  return buffer;
}

const uint8_t* ScreenBuffer::data() const
{
  return buffer;
}

void ScreenBuffer::setBuffer(const uint8_t* bitmap)
{
  memcpy_P(buffer, bitmap, SCREEN_BYTES);
}

bool ScreenBuffer::canDrawBitmap(uint8_t x, uint8_t y, uint8_t widthBits, uint8_t heightBits)
{
  return (
    x < SCREEN_WIDTH &&
    y < SCREEN_HEIGHT &&
    x + widthBits <= SCREEN_WIDTH &&
    y + heightBits <= SCREEN_HEIGHT
  );
}

// Set the value of the bit at coordinate (x,y) in the buffer
void ScreenBuffer::setPixel(uint8_t x, uint8_t y, bool pixelOn)
{
  if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
    Serial.println("Error in setPixel(): Bit not within bounds of screen.");
    return;
  }

  uint16_t byteIndex = y * SCREEN_BYTES_PER_ROW + (x / 8);
  uint8_t bitMask = 0x80 >> (x % 8);

  if (pixelOn) {
    buffer[byteIndex] |= bitMask;
  } else {
    buffer[byteIndex] &= ~bitMask;
  }
}

// Given a bitmap, its dimensions, and a top left starting coordinate, draw that bitmap onto the buffer
void ScreenBuffer::overlayBitmap(const uint8_t* bitmap, uint8_t x, uint8_t y, uint8_t widthBits, uint8_t heightBits) 
{
  if (!canDrawBitmap(x, y, widthBits, heightBits)) {
    Serial.println("Error in overlayBitmap(): Bitmap dimensions not within bounds of screen.");
    return;
  }

  if (widthBits % 8 != 0) {
    Serial.println("Error in overlayBitmap(): Bitmap width not divisible into byte size chunks.");
    return;
  }

  uint8_t bitmapBytesPerRow = widthBits / 8;

  for (uint8_t Y = 0; Y < heightBits; Y++) {
    for (uint8_t X = 0; X < widthBits; X++) {

      // Find the Bit location in the bitmap array
      uint16_t bitmapByteIndex = Y * bitmapBytesPerRow + (X / 8);
      uint8_t bitmapBitMask = 0x80 >> (X % 8);

      // Find out if that bit is 1 or 0
      uint8_t bitmapByte = pgm_read_byte(&bitmap[bitmapByteIndex]);
      bool pixelOn = bitmapByte & bitmapBitMask;

      setPixel(x + X, y + Y, pixelOn);
    }
  }
}