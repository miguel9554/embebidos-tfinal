#include "chip.h"
#include "board.h"

int main(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	int pinNamePort = 6;
	int pinNamePin  = 12;
	int gpioPort    = 2;
	int gpioPin     = 8;

	Chip_SCU_PinMux(
	            pinNamePort,
	            pinNamePin,
	            SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
				SCU_MODE_FUNC0
	         );
	Chip_GPIO_SetPinDIROutput( LPC_GPIO_PORT, gpioPort, gpioPin);
	Chip_GPIO_SetPinOutLow( LPC_GPIO_PORT, gpioPort, gpioPin);


	while(1){

	}
}
