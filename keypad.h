#pragma once

#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "oledDisplay.h"

extern char PASSWORD[6];
#define ACCESS_GRANTED 1
#define NO_ACCESS_GRANTED 0
#define PASSWORD_LENGHT 5
#define DEFAULT_READ_KEY 0x1

void delay_ms_soft(uint32_t ms);
#if defined(STM32F407xx)

#define R1_PORT GPIOA
#define R1_PIN GPIO_PIN_7

#define R2_PORT GPIOA
#define R2_PIN GPIO_PIN_6

#define R3_PORT GPIOA
#define R3_PIN GPIO_PIN_5

#define R4_PORT GPIOA
#define R4_PIN GPIO_PIN_4

#define C1_PORT GPIOA
#define C1_PIN GPIO_PIN_3

#define C2_PORT GPIOA
#define C2_PIN GPIO_PIN_2

#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_1

#define C4_PORT GPIOA
#define C4_PIN GPIO_PIN_0

#elif defined(STM32F411xE)

#define R1_PORT GPIOA
#define R1_PIN GPIO_PIN_7

#define R2_PORT GPIOB
#define R2_PIN GPIO_PIN_6

#define R3_PORT GPIOC
#define R3_PIN GPIO_PIN_7

#define R4_PORT GPIOA
#define R4_PIN GPIO_PIN_9

#define C1_PORT GPIOB
#define C1_PIN GPIO_PIN_1

#define C2_PORT GPIOB
#define C2_PIN GPIO_PIN_15

#define C3_PORT GPIOB
#define C3_PIN GPIO_PIN_14

#define C4_PORT GPIOB
#define C4_PIN GPIO_PIN_13

#endif
char* enterPassword();
void handlePasswordChange();
extern char key;
char read_keypad(void);
