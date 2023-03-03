#pragma once

#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

//extern uint32_t IC_Val1 = 0;
//extern uint32_t IC_Val2 = 0;
//extern uint32_t Difference = 0;
//extern uint8_t Is_First_Captured = 0; // is the first value captured
extern uint8_t Distance;

#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOC

void delay(uint16_t time);
uint8_t US_Init();
void US_Read(void);
