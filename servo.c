#include "servo.h"

TIM_HandleTypeDef htim2;
struct servoParameters servo1;
struct servoParameters servo2;

uint8_t Servo_TIM2_Init()
{
#if defined(STM32F407xx)

	// TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	// TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1680000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	__HAL_RCC_TIM2_CLK_ENABLE();

	TIM_Base_SetConfig(htim2.Instance, &htim2.Init);

	// if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	// {
	// 	return -1;
	// }
	// if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	// {
	// 	return -1;
	// }

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		return -1;
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		return -1;
	}

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/*
	TIM2 GPIO Configuration
	PA1 ------> TIM2_CH2
	PA15 ------> TIM2_CH1
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#elif defined(STM32F411xE)

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	//	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	// wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
	// Initialize pin for servo1 & servo 2
	//-------------------------------------------------------------------------------

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/*
	TIM2 GPIO Configuration
	PA0-WKUP ------> TIM2_CH1
	PA1 ------> TIM2_CH2
	*/
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
	// Configure timer
	//-------------------------------------------------------------------------------

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 15 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 60000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	__HAL_RCC_TIM2_CLK_ENABLE();

	// TIM_Base_SetConfig(htim2.Instance, &htim2.Init);

	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		return -1;
	}

	// funkcije HAM_TIM_PWM_Init i HAL_TIM_Base_Init haman iste stvari cine sem sto Base_Init enablira TIM2_CLK

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		return -1;
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		return -1;
	}

	//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	//	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	//	{
	//	  return -1;
	//	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		return -1;
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		return -1;
	}

#endif

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // start the PWM for servo1(ulazni gate)
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // start the PWM for servo2(izlazni gate)

	servo1.start = 1050;
	servo1.end = 7700;
	servo1.channel = CHANNEL1;
//380, 2750
	servo2.start = 1050;
	servo2.end = 7700;
	servo2.channel = CHANNEL2;

	setAngle(0, &servo1);
	//	setAngle(0, &servo2);

	return 0;
}

void setCCR(uint32_t CCR, struct servoParameters *servo)
{
	if (servo->channel == CHANNEL1)
		htim2.Instance->CCR1 = CCR;
	else if (servo->channel == CHANNEL2)
		htim2.Instance->CCR2 = CCR;
}

uint32_t getCCR(struct servoParameters *servo)
{
	if (servo->channel == CHANNEL1)
		return htim2.Instance->CCR1;
	else if (servo->channel == CHANNEL2)
		return htim2.Instance->CCR2;
}

void changeCCR(uint32_t CCR, struct servoParameters *servo)
{

	if (servo->channel == CHANNEL1)
		htim2.Instance->CCR1 += CCR;
	else if (servo->channel == CHANNEL2)
		htim2.Instance->CCR2 += CCR;
}

uint8_t setAngle(uint8_t angle, struct servoParameters *servo)
{
	if (angle > 180)
		return -1;

	float scale = angle * 1. / 180;
	if (servo->channel == CHANNEL1)
		htim2.Instance->CCR1 = (servo->start * (1 - scale) + servo->end * scale);
	else if (servo->channel == CHANNEL2)
		htim2.Instance->CCR2 = (servo->start * (1 - scale) + servo->end * scale);

	return 0;
}
