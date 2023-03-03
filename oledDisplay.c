#include "oledDisplay.h"

#if defined(STM32F407xx)
I2C_HandleTypeDef hi2c3;
#elif defined(STM32F411xE)
I2C_HandleTypeDef hi2c2;
#endif

#define WRITECOMMAND(command) I2C_Write(I2C_ADDR, 0x00, (command))
#define WRITEDATA(data) I2C_Write(I2C_ADDR, 0x40, (data))

static uint8_t Buffer[WIDTH * HEIGHT / 8];

static uint8_t CurrentX = 0;
static uint8_t CurrentY = 0;

const uint8_t COLOR_BLACK = 0X00;
const uint8_t COLOR_WHITE = 0X01;

uint8_t OLED_Init(void)
{
	if (OLED_I2C_Init() < 0)
		return -1;

	if (HAL_I2C_IsDeviceReady(&hi2c2, I2C_ADDR, 1, 20000) != HAL_OK)
		return -1;

	WRITECOMMAND(0xA8); // Set MUX Ratio
	WRITECOMMAND(0x3F);
	WRITECOMMAND(0xD3); // Set Display Offset
	WRITECOMMAND(0x00);
	WRITECOMMAND(0x40); // Set Display Start Line
						//	WRITECOMMAND(0xA0); // Set Segment re-map
	WRITECOMMAND(0xA1); // Set Segment re-map inverse (which actually maps it intuitively for humans)
						//	WRITECOMMAND(0xC0); // Set COM Output Scan Direction
	WRITECOMMAND(0xC8); // (intuitive direction also)
	WRITECOMMAND(0xDA); // Set COM Pins hardware configuration
	WRITECOMMAND(0x12);
	WRITECOMMAND(0x81); // Set Contrast Control
	WRITECOMMAND(0x7F);
	WRITECOMMAND(0xA4); // Disable Entire Display On
	WRITECOMMAND(0xA6); // Set Normal Display
	WRITECOMMAND(0xD5); // Set Osc Frequency
	WRITECOMMAND(0x80);
	WRITECOMMAND(0x8D); // Enable charge pump regulator
	WRITECOMMAND(0x14);
	WRITECOMMAND(0xAF); // Display On
						//	WRITECOMMAND(0xF0); // Set divide ratio
						//	WRITECOMMAND(0xD9); // Set pre-charge period
						//	WRITECOMMAND(0x22);

	FillScreen(COLOR_BLACK);
	UpdateScreen();
	CurrentX = 0;
	CurrentY = 0;

	return 0; // Return OK
}

void UpdateScreen(void)
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		WRITECOMMAND(0xB0 + i); // Set Page Start Address (B0h ~ B7h)
		WRITECOMMAND(0x00);		// Set Lower Column Start Address
		WRITECOMMAND(0x10);		// Set Higher Column Start Address
		I2C_Write_Array(I2C_ADDR, 0x40, &Buffer[WIDTH * i], WIDTH);
	}
}

void FillScreen(uint8_t color)
{
	memset(Buffer, (color == COLOR_WHITE) ? 0xFF : 0x00, sizeof(Buffer));
}

uint8_t DrawPixel(uint16_t x, uint16_t y, uint8_t color)
{
	if (x >= WIDTH || y >= HEIGHT)
	{
		return -1;
	}

	if (color)
	{
		Buffer[x + (y / 8) * WIDTH] |= 0x1 << (y % 8);
	}
	else
	{
		Buffer[x + (y / 8) * WIDTH] &= ~(1 << (y % 8));
	}
	return 0;
}

uint8_t Goto(uint16_t x, uint16_t y)
{
	if (x < 0 || x >= (WIDTH - 1) || y < 0 || y >= (HEIGHT - 1))
		return -1;
	CurrentX = x;
	CurrentY = y;
	return 0;
}

uint8_t PutChar(char ch)
{
	uint8_t fontWidth = 7;
	uint8_t fontHeight = 10;
	if ((CurrentX + fontWidth) >= WIDTH || (CurrentY + fontHeight) >= HEIGHT)
		return -1;

	uint16_t hw; // halfword
	for (uint8_t i = 0; i < fontHeight; i++)
	{
		hw = font710[(ch - 32) * fontHeight + i];
		for (uint8_t j = 0; j < fontWidth; j++)
		{
			if ((hw << j) & 0x8000)
			{
				DrawPixel(CurrentX + j, (CurrentY + i), COLOR_WHITE);
			}
			else
			{
				DrawPixel(CurrentX + j, (CurrentY + i), COLOR_BLACK);
			}
		}
	}
	CurrentX += fontWidth;
	return 0; // success, return OK
}

uint8_t PrintString(char *str)
{
	char *counter = str;
	while (*counter)
	{
		if (PutChar(*counter) < 0)
		{
			return -1;
		}
		++counter;
	}
	return 0;
}

uint8_t I2C_Write_Array(uint8_t address, uint8_t code, uint8_t *data, uint16_t count)
{
	uint8_t packet[256];
	packet[0] = code;
	for (uint8_t i = 0; i < count; ++i)
		packet[i + 1] = data[i];
	if (HAL_I2C_Master_Transmit(&hi2c2, address, packet, count + 1, 10) != HAL_OK)
		return -1;
	return 0;
}

uint8_t I2C_Write(uint8_t address, uint8_t code, uint8_t data)
{
	uint8_t packet[2];
	packet[0] = code;
	packet[1] = data;
	if (HAL_I2C_Master_Transmit(&hi2c2, address, packet, 2, 10) != HAL_OK)
		return -1;
	return 0;
}

uint8_t OLED_I2C_Init()
{

#if defined(STM32F407xx)

	hi2c3.Instance = I2C3;
	hi2c3.Init.ClockSpeed = 400000;
	hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/*
	I2C3 GPIO Configuration
	PC9 ------> I2C3_SDA
	PA8 ------> I2C3_SCL
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Peripheral clock enable */
	__HAL_RCC_I2C3_CLK_ENABLE();

	if (HAL_I2C_Init(&hi2c3) != HAL_OK)
	{
		return -1;
	}

#elif defined(STM32F411xE)
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 400000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*
	I2C2 GPIO Configuration
	PB10 ------> I2C2_SCL
	PB9 ------> I2C2_SDA
	*/

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_I2C2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Peripheral clock enable */
	__HAL_RCC_I2C2_CLK_ENABLE();

	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		return -1;
	}
// HAL_I2C_DeInit(&hi2c1);
#endif

	return 0;
}
