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
	            SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
				SCU_MODE_FUNC0
	         );
	Chip_GPIO_SetPinDIROutput( LPC_GPIO_PORT, gpioPort, gpioPin);
	Chip_GPIO_SetPinOutLow( LPC_GPIO_PORT, gpioPort, gpioPin);

	tickConfig( 1 );


	while(1){
		Chip_GPIO_SetPinOutLow( LPC_GPIO_PORT, gpioPort, gpioPin);
		delay(1000);
		Chip_GPIO_SetPinOutHigh( LPC_GPIO_PORT, gpioPort, gpioPin);
		delay(1000);
	}
}
