#include "sapi.h"

int main(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	int pinNamePort = 6;
	int pinNamePin  = 1;
	int gpioPort    = 3;
	int gpioPin     = 0;

	Chip_SCU_PinMux(
	            pinNamePort,
	            pinNamePin,
				SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,
				SCU_MODE_FUNC0
	         );

	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioPort, gpioPin);

	tickConfig( 1 );


	while(1){
		gpioWrite( LED1, Chip_GPIO_GetPinState(LPC_GPIO_PORT, gpioPort, gpioPin));
		delay(100);
	}
}
