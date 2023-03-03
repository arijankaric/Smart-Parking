#include "keypad.h"

char key;
char PASSWORD[6] = "#1111";

char read_keypad()
{
	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
			;
		return '1';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
			;
		return '2';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
			;
		return '3';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
			;
		return 'A';
	}

	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
			;
		return '4';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
			;
		return '5';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
			;
		return '6';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
			;
		return 'B';
	}

	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
			;
		return '7';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
			;
		return '8';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
			;
		return '9';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
			;
		return 'C';
	}

	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
			;
		return '*';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
			;
		return '0';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
			;
		return '#';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
	{
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
			;
		return 'D';
	}

	return 0x01;
}
void clearPassword(char *currentPassword, int *currentPasswordLenght)
{
	*currentPasswordLenght = 0;
	currentPassword[0] = '\0';
	currentPassword[1] = '\0';
	currentPassword[2] = '\0';
	currentPassword[3] = '\0';
	currentPassword[4] = '\0';
}

void overwriteCurrentState(char *message)
{
	// TODO arijane pls pogledaj ovde koji bi offset pixela godio da krene string od njega
	// P.S. ja se nadam da prebrise citav ekran ovaj printString
	Goto(0, 0);
	PrintString(message);
}

char* enterPassword()
{
	char readKey;
	uint i = 0;
	char *pw = malloc(6);
	memset(pw, 0, 5);
	Goto(0, 30);
	while (i < 5)
	{
		readKey = read_keypad();
		if (readKey != 0x01)
		{
			// PutChar(readKey);
			PutChar('n');
			// sprintf(str, "%c", readKey);
			// PrintString(readKey);
			pw[i] = readKey;
			UpdateScreen();
			++i;
		}
	}
	pw[i] = '\0';
	return pw;
}

void handlePasswordChange()
{
	Goto(0, 0);
	PrintString("changing pw");
	Goto(0, 10);
	PrintString("enter curr pw");
	// Goto(0, 20);
	// PrintString("changing pw");
	UpdateScreen();
	// FillScreen(0);

	char *newPw = enterPassword();

	if (strcmp(newPw, PASSWORD) == 0)
	{
		FillScreen(0);
		// PrintString("enter new pw");
		Goto(0, 20);
		PrintString("enter new pw");
		UpdateScreen();

		char *newPw = enterPassword();
		strcpy(PASSWORD, newPw);
		Goto(0, 40);
		PrintString("new pw: OK");
		UpdateScreen();
		FillScreen(0);
		HAL_Delay(850);
	}
	else
	{
		FillScreen(0);
		Goto(0, 30);
		PrintString("incorrect pw");
		UpdateScreen();
		FillScreen(0);
		HAL_Delay(850);
	}
}
