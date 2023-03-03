#include "keypad.h"


//Main bi mogao izgledai otprilike ovako

int main(void)
{

	//HAL_API_INIT

	char[5] password;
	int passwordLenght = 0;
	int parkingAccess = NO_ACCESS_GRANTED:
	while (1)
	{
		delay_ms_soft(100);
		parkingAccess = handlePasswordChange(password,&passwordLenght); 
		handleRampActivity(parkingAccess); //->Ovo bi moglo doci iz ramp.h or smth

		//putcharUSART2(read_keypad());
	}
    return 0 ;
}

void delay_ms_soft(uint32_t ms)
{
	volatile uint32_t k = 10500 * ms;
	while (k--)
		;
}

