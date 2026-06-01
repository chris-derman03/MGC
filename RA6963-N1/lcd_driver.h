#pragma once
#include <Arduino.h>
#include <stdint.h>

// Resolution
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
#define SCREEN_BYTES ((SCREEN_WIDTH * SCREEN_HEIGHT) / 8)

void lcd_setup();
void initialize_display();
void clearDisplay();
void displayBuffer(const uint8_t* buffer);