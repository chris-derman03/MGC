#pragma once
#include <stdint.h>

void lcd_setup();
void initialize_display();
void clearDisplay();
void displayFullscreenByteArray(const uint8_t *fullscreenByteArray);