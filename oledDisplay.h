#pragma once

#include "stdint.h"

/*

SSD1306    |STM32F407    |DESCRIPTION

VCC        |3.3V         |
GND        |GND          |
SCL        |PB6          |Serial clock line
SDA        |PB7          |Serial data line

*/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_i2c.h"
#include <stdlib.h>
#include "string.h"
#include "fonts1.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define I2C_ADDR 0x78
//#define I2C_ADDR 0x7A
#define WIDTH 128
#define HEIGHT 64

extern const uint8_t COLOR_BLACK;
extern const uint8_t COLOR_WHITE;



uint8_t OLED_Init(void);
void UpdateScreen(void);
void FillScreen(uint8_t Color);
uint8_t DrawPixel(uint16_t x, uint16_t y, uint8_t Color);
uint8_t Goto(uint16_t x, uint16_t y);
uint8_t PutChar(char ch);
uint8_t PrintString(char *str);
uint8_t OLED_I2C_Init();
uint8_t I2C_Write_Array(uint8_t address, uint8_t code, uint8_t* data, uint16_t count); // multiple bytes write
uint8_t I2C_Write(uint8_t address, uint8_t code, uint8_t data);
