#include "obstacleSensor.h"


void initObstacleSensor(uint32_t pin, GPIO_TypeDef  *port)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
   	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}


int isParkingOccupied (uint32_t pin, GPIO_TypeDef  *port){
    return HAL_GPIO_ReadPin(port,pin) == 0;
}
