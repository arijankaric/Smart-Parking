#ifndef OBSTACLESENSOR_H
#define OBSTACLESENSOR_H

#define PARKING_SENSOR_1_PIN GPIO_PIN_6
#define PARKING_SENSOR_1_PORT GPIOE

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

void initObstacleSensor(uint32_t pin,GPIO_TypeDef  *port);
int isParkingOccupied ( uint32_t pin,GPIO_TypeDef  *port);


#endif
