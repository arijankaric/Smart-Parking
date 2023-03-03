#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "obstacleSensor.h"
void delay_ms_soft(uint32_t ms);

int main(void)
{
 	GPIO_InitTypeDef GPIO_InitStruct;
   	initObstacleSensor(&GPIO_InitStruct,PARKING_SENSOR_1_PIN,PARKING_SENSOR_1_PORT);
	
	//diode za test
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//usart za test
	//initUSART2(921600);
	
	//printUSART2("USLO U PROGRAM\n");

    int k = 0;
	while(1)
	{
	         // delay_ms_soft(500);
          if(isParkingOccupied(PARKING_SENSOR_1_PIN,PARKING_SENSOR_1_PORT))
          {
            //k++;
            //printUSART2("Detektovo %d\n", k);
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,0x01);
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,0x01);
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,0x01);
	    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,0x01);
          }
          else
          {
	      //printUSART2("Nije detektovo\n");
	     HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,0x00);
	     HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,0x00);
             HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,0x00);
	     HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,0x00);
	  }
	}
}


void delay_ms_soft(uint32_t ms)
{
	volatile uint32_t k = 10500 * ms;
	while(k--);
}
