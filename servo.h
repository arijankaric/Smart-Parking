#pragma once

#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define CHANNEL1 0
#define CHANNEL2 1

// za F407
// 1'680'000 => 100%
// prescaler 1(odnosno 1 - 1 = 0)
// 16800 => 1%
// 33600 => 2%
// 168'000 => 10% 2ms
// 84'000 => 5% 1ms

// za F411RE
// 60'000 => 100%
// prescaler = 15 (odnosno 15 - 1 = 14)
// 6000 => 10% 2ms
// 3000 => 5% 1ms

// 1039 za servo1 0 degree
// negdje oko 7800 za servo1 180 degree

struct servoParameters
{
	float start;
	float end;
	uint8_t channel;
};

extern struct servoParameters servo1;
extern struct servoParameters servo2;

//extern TIM_HandleTypeDef htim2;

uint8_t Servo_TIM2_Init();
uint8_t setAngle(uint8_t angle, struct servoParameters *servo);
void setCCR(uint32_t CCR, struct servoParameters *servo);
uint32_t getCCR(struct servoParameters *servo);
void changeCCR(uint32_t CCR, struct servoParameters *servo);
