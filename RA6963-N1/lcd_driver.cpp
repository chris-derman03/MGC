//  The controller is a RAiO RA6963-N1
//    https://www.crystalfontz.com/controllers/Raio%20Technology/RA6963/424
//
//
//==============================================================================
// LCD & USD control lines
//   ARD      | Port  | Display pin |  Function - 8080 Parallel   |
//------------+-------+-------------+-----------------------------+
//  5V        |       |             |  POWER                      |
//  GND	      |       |             |  GROUND                     |
// -----------+-------+-------------+-----------------------------+
//  D8        | PORTB |             |  Chip Enable Signal  (CS)   |
//  D9        | PORTB |             |  Reset            (RESET)   |
//  D10       | PORTB |             |  Data/Command        (DC)   |
//  D11       | PORTB |             |  Write               (WR)   |
//  D12       | PORTB |             |  Read                (RD)   |
// -----------+-------+-------------+-----------------------------+
// Data Lines
// -----------+-------+-------------+-----------------------------+
//  D20       | PORTD |             |  LCD_D10                    |
//  D21       | PORTD |             |  LCD_D11                    |
//  D2        | PORTD |             |  LCD_D12                    |
//  D3        | PORTD |             |  LCD_D13                    |
//  D4        | PORTD |             |  LCD_D14                    |
//  D5        | PORTD |             |  LCD_D15                    |
//  D6        | PORTD |             |  LCD_D16                    |
//  D7        | PORTD |             |  LCD_D17                    |
// -----------+-------+-------------+-----------------------------+


//==============================================================================
#include "lcd_driver.h"
#include <avr/pgmspace.h>
#include <util/delay.h>

//==============================================================================
// GPIO
//==============================================================================
#define LCD_CS     8
#define LCD_RESET  9
#define LCD_DC     10
#define LCD_WR     11
#define LCD_RD     12

#define LCD_CS_ENABLE     digitalWrite(LCD_CS, LOW)
#define LCD_CS_DISABLE    digitalWrite(LCD_CS, HIGH)
#define LCD_RESET_ON      digitalWrite(LCD_RESET, LOW)
#define LCD_RESET_OFF     digitalWrite(LCD_RESET, HIGH)
#define LCD_DATA_MODE     digitalWrite(LCD_DC, LOW)
#define LCD_COMMAND_MODE  digitalWrite(LCD_DC, HIGH)
#define LCD_WRITE_ACTIVE  digitalWrite(LCD_WR, LOW)
#define LCD_WRITE_IDLE    digitalWrite(LCD_WR, HIGH)
#define LCD_READ_ACTIVE   digitalWrite(LCD_RD, LOW)
#define LCD_READ_IDLE     digitalWrite(LCD_RD, HIGH)

const uint8_t DATA_PINS[8] = {20,21,2,3,4,5,6,7};

//==============================================================================
// LCD Controls
//==============================================================================
#define GRAPHICS_HOME           0x0000

#define ENABLE_GRAPHICS         writeCommand(0x98);
#define SET_ADDRESS_POINTER     writeCommand(0x24); // To previous 2 bytes sent
#define ENABLE_AUTO_WRITE       writeCommand(0xB0);
#define DISABLE_AUTO            writeCommand(0xB2);

// Write a Byte to the data pins
void writeBus(uint8_t value)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(DATA_PINS[i], (value >> i) & 1);
  }
}

void writeCommand(uint8_t command)
{
  LCD_CS_ENABLE;

  LCD_COMMAND_MODE;

  writeBus(command);

  LCD_WRITE_ACTIVE;
  LCD_WRITE_IDLE;

  LCD_CS_DISABLE;
}

void writeData(uint8_t data)
{
  LCD_CS_ENABLE;

  LCD_DATA_MODE;
  
  writeBus(data);

  LCD_WRITE_ACTIVE;
  LCD_WRITE_IDLE;

  LCD_CS_DISABLE;
}

void setAddressPointer(uint16_t address)
{
  writeData(address & 0xFF);
  writeData((address >> 8) & 0xFF);
  SET_ADDRESS_POINTER;
}

//==============================================================================
void lcd_setup()
{
  for (int i = 0; i < 8; i++) {
    pinMode(DATA_PINS[i], OUTPUT);
    digitalWrite(DATA_PINS[i], HIGH);
  }

  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RESET, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);

  LCD_READ_IDLE;
  LCD_WRITE_IDLE;
  LCD_CS_DISABLE;
  delay(1);
}

void initialize_display()
{
  LCD_RESET_OFF;
  _delay_ms(10);
  LCD_RESET_ON;
  _delay_ms(100);
  LCD_RESET_OFF;
  _delay_ms(500);

  // Set graphics home address
  writeData(GRAPHICS_HOME & 0xFF);
  writeData((GRAPHICS_HOME >> 8) & 0xFF);
  writeCommand(0x42);

  // Set graphics area
  writeData(SCREEN_WIDTH / 8);
  writeData(0x00);
  writeCommand(0x43);

  ENABLE_GRAPHICS;
}

//==============================================================================
// Display Logic
//==============================================================================
void clearDisplay()
{
  setAddressPointer(GRAPHICS_HOME);
  ENABLE_AUTO_WRITE; 

  uint16_t i = SCREEN_BYTES;
  while (i--) {
    writeData(0x00);
  }

  DISABLE_AUTO;
}

void displayBuffer(const uint8_t *buffer)
{
  setAddressPointer(GRAPHICS_HOME);
  ENABLE_AUTO_WRITE;

  uint16_t i = SCREEN_BYTES;
  while (i--) {
    writeData(*buffer++);
  }

  DISABLE_AUTO;
}