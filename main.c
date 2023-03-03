#include "main.h"
#include "fonts1.h"
#include "oledDisplay.h"
#include "servo.h"
#include <stdio.h>
#include <math.h>
#include "STM32_Init.h"
#include "ultrasound.h"
#include "keypad.h"
#include <stdint.h>
#include <string.h>
#include "fatfs_sd.h"
#include "ff.h"
#include "user_diskio.h"
#include "fatfs.h"
#include "obstacleSensor.h"

// #define testing 0

FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t total, freeS;
#define ROW_MAX_BUFFER 18
char buffer[ROW_MAX_BUFFER];
char str[ROW_MAX_BUFFER];
char numStr[100];
const uint8_t totalSpace = 3;

float Temperature, Humidity;

int main(void)
{
	Init();
	uint8_t a;
	uint8_t occupied;
	uint8_t free;
	float hold;
	float varijablaTim;
	uint32_t IC_Val2 = 0;
	uint32_t IC_Val1 = 0;
	uint32_t Diff;
	uint32_t tickfreq;
	// __HAL_TIM_SET_COUNTER(&htim1, 0);
	// for(;;)
	// {
	// 	US_Read();
	// 	memset(buffer, 0, sizeof(buffer));
	// 	sprintf(buffer, "%d", Distance);
	// 	Goto(0, 0);
	// 	PrintString(buffer);
	// 	UpdateScreen();
	// 	FillScreen(0);
	// }
	while (1)
	{
		occupied = isParkingOccupied(PARKING_SENSOR_1_PIN, PARKING_SENSOR_1_PORT);
		free = totalSpace - occupied;
		// occupied = isParkingOccupied(PARKING_SENSOR_2_PIN, PARKING_SENSOR_2_PORT);
		// free = total - occupied;
		// occupied = isParkingOccupied(PARKING_SENSOR_3_PIN, PARKING_SENSOR_3_PORT);
		// free = total - occupied;
		Goto(0, 0);
		PrintString("Free: ");
		sprintf(buffer, "%d", free);
		PrintString(buffer);
		Goto(0, 30);
		PrintString("Time: ");
		varijablaTim = uwTick / 319.0;
		sprintf(buffer, "%f", varijablaTim);
		PrintString(buffer);
		UpdateScreen();
		FillScreen(0);
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) // izlazni gumb
		{
			while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1))
				;
			fres = f_mount(&fs, "", 0); // 1=mount now
			if (fres != FR_OK)
			{
				while (1)
				{
					Goto(0, 0);
					PrintString("no microsd");
					UpdateScreen();
					FillScreen(0);
					fres = f_mount(&fs, "", 0); // 1=mount now
					if (fres == FR_OK)
						break;
				}
			}
			setAngle(90, &servo2);
			fres = f_open(&fil, "stay.txt", FA_READ);

			uint8_t i = 0;
			memset(buffer, 0, sizeof(buffer));
			f_gets(buffer, sizeof(buffer), &fil);
			Goto(0, 0);
			PrintString("ulaz: ");
			varijablaTim = uwTick / 319.0;
			hold = varijablaTim - atoi(buffer);
			PrintString(buffer);
			Goto(0, 20);
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "%f", varijablaTim);
			PrintString("izlaz: ");
			PrintString(buffer);
			Goto(0, 30);
			PrintString("razlika: ");
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "%f", hold);
			PrintString(buffer);
			Goto(0, 40);
			PrintString("cijena ");
			hold = 0.5 + (trunc(hold / 30.)) * 0.5;
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "%f", hold);
			PrintString(buffer);
			UpdateScreen();
			memset(buffer, 0, sizeof(buffer));
			FillScreen(0);
			fres = f_close(&fil);
			fres = f_mount(NULL, "", 1);
			HAL_Delay(2000);
			setAngle(0, &servo2);
		}
		else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)) // ulazni gumb
		{
			IC_Val1 = uwTick;
			while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0))
			{
			}
			IC_Val2 = uwTick;
			Diff = IC_Val2 - IC_Val1;
			hold = Diff / 319.00;
			if (hold < 3)
			{
				Goto(0, 0);
				PrintString("ulaz ");
				PrintString(str);
				Goto(0, 30);
				PrintString("enter pw ");
				UpdateScreen();
				FillScreen(0);
				char *newPw = enterPassword();
				// FillScreen(0);
				// Goto(0, 40);
				// PrintString(pw);
				// Goto(0, 50);
				// PrintString(PASSWORD);
				// UpdateScreen();
				// FillScreen(0);
				// HAL_Delay(5000);

				if (strcmp(newPw, PASSWORD) == 0)
				{
					setAngle(90, &servo1);
					fres = f_mount(&fs, "", 0); // 1=mount now
					fres = f_open(&fil, "stay.txt", FA_CREATE_ALWAYS | FA_WRITE);
					varijablaTim = uwTick / 319.0;
					sprintf(str, "%f", varijablaTim);
					f_puts(str, &fil);
					Goto(0, 30);
					PrintString("correct pw ");
					fres = f_close(&fil);
					fres = f_mount(NULL, "", 1);
					HAL_Delay(2000);
					US_Read();
					while (Distance < 12)
					{
						US_Read();
						HAL_Delay(300);
					}
					setAngle(0, &servo1);
				}
				else
				{
					Goto(0, 30);
					PrintString("incorrect pw ");
					UpdateScreen();
					FillScreen(0);
					HAL_Delay(650);
				}
			}
			else if (hold >= 3)
			{
				handlePasswordChange();
				// change password
			}
		}
		// IC_Val1 = IC_Val2;
		// IC_Val2 = uwTick;
		// if (IC_Val2 > IC_Val1)
		// {
		// 	Difference = IC_Val2 - IC_Val1;
		// }
		// else if (IC_Val1 > IC_Val2)
		// {
		// 	Difference = (0xffff - IC_Val1) + IC_Val2;
		// }
		// varijablaTim += Difference;
		// a = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
		// hold = varijablaTim / 319.00;
		// if (a)
		// {
		// 	Goto(0, 0);
		// 	sprintf(str, "%f", hold);
		// 	sprintf(buffer, "%f", tickfreq);
		// 	PrintString("upaljen ");
		// 	PrintString(str);
		// 	Goto(0, 30);
		// 	PrintString(buffer);
		// 	UpdateScreen();
		// 	FillScreen(0);
		// }
		// else
		// {
		// 	varijablaTim = 0;
		// 	Goto(0, 0);
		// 	PrintString("ugasen");
		// 	UpdateScreen();
		// 	FillScreen(0);
		// }
		//	  uint8_t angle = 0;
		//	  while(angle < 180)
		//	  {
		//		  setAngle(angle++, &servo1);
		//	  }
		//	  angle = 0;

		// setCCR(200, &servo2);
		// uint32_t ccr;
		// while (getCCR(&servo2) < 2900)
		// {
		// 	changeCCR(10, &servo2);
		// 	Goto(0, 0);
		// 	ccr = getCCR(&servo2);
		// 	sprintf(str, "%d", ccr);
		// 	PrintString(str);
		// 	UpdateScreen();
		// 	FillScreen(0);
		// 	HAL_Delay(1000);
		// }

		//	  htim2.Instance->CCR1 = 3000;
		//	  HAL_Delay(3000);
		//	  htim2.Instance->CCR1 = 4500;
		//	  HAL_Delay(3000);
		//	  htim2.Instance->CCR1 = 6000;
		//	  HAL_Delay(3000);

		//   key = read_keypad ();

		// 	  if (key!=0x01)
		// 	  {
		// //		  sprintf(str, "%d", key);
		// 		  Goto(0, 30);
		// 		  PrintString(&key);
		// 		  PrintString(" pritisnuto");
		// 		  UpdateScreen();
		// 		  FillScreen(0);
		// 	  }

		//   US_Read();
		//   sprintf(str, "%d", Distance);
		//   Goto(0, 0);
		//   PrintString(str);
		//   PrintString(" cm");
		//   UpdateScreen();
		//   FillScreen(0);

		//   if(Distance < 10)
		//   	  setAngle(0, &servo1);
		//   else if(Distance < 20)
		// 	  setAngle(90, &servo1);
		//   else
		// 	  setAngle(180, &servo1);
		//
		//	  HAL_Delay(1500);

		//	  if(Distance < 10)
		//		  setCCR1(1039);
		//	  else if(Distance < 20)
		//		  setCCR1((1039+7800)/2);
		//	  else
		//		  setCCR1(7800);
	}
}

void Init()
{

	HAL_Init();
	uint8_t STMRES = STM32_Init();
	uint8_t OLEDRES = OLED_Init(); // initialise

#if defined(testing)
	Goto(0, 0);
	sprintf(str, "%d", STMRES);
	PrintString("STMRES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	Goto(0, 0);
	sprintf(str, "%d", OLEDRES);
	PrintString("OLEDRES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);
#endif

	fres = US_Init();
#if defined(testing)
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("USRES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	US_Read();

	Goto(0, 0);
	sprintf(str, "%d", Distance);
	PrintString("Distance1: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	US_Read();

	Goto(0, 0);
	sprintf(str, "%d", Distance);
	PrintString("Distance2: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	US_Read();

	Goto(0, 0);
	sprintf(str, "%d", Distance);
	PrintString("Distance3: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);
#endif

	fres = FATFS_Init();
#if defined(testing)
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("FATFSRES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);
#endif

	initObstacleSensor(PARKING_SENSOR_1_PIN, PARKING_SENSOR_1_PORT);
#if defined(testing)
	fres = isParkingOccupied(PARKING_SENSOR_1_PIN, PARKING_SENSOR_1_PORT);
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("OCCUPIED1: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	fres = isParkingOccupied(PARKING_SENSOR_1_PIN, PARKING_SENSOR_1_PORT);
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("OCCUPIED2: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);
#endif

	fres = Servo_TIM2_Init();
#if defined(testing)
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("SERVORES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	setAngle(90, &servo1);
	Goto(0, 0);
	// sprintf(str, "%d", fres);
	PrintString("SERVO1: 90degrees");
	// PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	setAngle(180, &servo1);
	Goto(0, 0);
	// sprintf(str, "%d", fres);
	PrintString("SERVO1: 180degrees");
	// PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	setAngle(0, &servo1);
	setAngle(90, &servo2);
	Goto(0, 0);
	// sprintf(str, "%d", fres);
	PrintString("SERVO1: 0degrees");
	Goto(0, 20);
	PrintString("SERVO2: 90degrees");
	// PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	setAngle(180, &servo2);
	Goto(0, 0);
	// sprintf(str, "%d", fres);
	PrintString("SERVO1: 0degrees");
	Goto(0, 20);
	PrintString("SERVO2: 180degrees");
	// PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);
	setAngle(0, &servo2);

	fres = f_mount(&fs, "", 0); // 1=mount now
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("MOUNTRES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	DWORD free_clusters, free_sectors, total_sectors;

	FATFS *getFreeFs;
	fres = f_getfree("", &fre_clust, &pfs);
	Goto(0, 0);
	sprintf(str, "%d", fres);
	PrintString("FREERES: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	// Formula comes from ChaN's documentation
	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	freeS = (uint32_t)(fre_clust * pfs->csize * 0.5);

	sprintf(str, "%u", total);
	Goto(0, 0);
	PrintString("Total: ");
	PrintString(str);
	// PrintString("");
	sprintf(str, "%u", freeS);
	Goto(0, 10);
	PrintString("Free: ");
	PrintString(str);
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);

	/* Open file to write */
	fres = f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	/* Writing text */
	f_puts("STM32 SD Card I/O Example via SPI\n", &fil);
	f_puts("Save the world!!!", &fil);

	/* Close file */
	fres = f_close(&fil);

	/* Open file to read */
	fres = f_open(&fil, "first.txt", FA_READ);

	uint8_t i = 0;
	while (f_gets(buffer, sizeof(buffer), &fil))
	{
		Goto(0, 20 + i * 10);
		++i;
		PrintString(buffer);
		UpdateScreen();
		HAL_Delay(INIT_DELAY);
		memset(buffer, 0, sizeof(buffer));
		//    		printf("%s", buffer);
		//    		fflush(stdout);
	}
	FillScreen(0);

	fres = f_close(&fil);

	/* Unmount SDCARD */
	fres = f_mount(NULL, "", 1);

	Goto(0, 0);
	PrintString("init finished");
	UpdateScreen();
	FillScreen(0);
	HAL_Delay(INIT_DELAY);
#endif
}