/* Copyright 2016, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of Workspace.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
/** @brief This is a simple blink example.
 */

/** \addtogroup blink Bare-metal blink example
 ** @{ */

/*==================[inclusions]=============================================*/

#include "ds18b20.h"
#include "sapi.h"
DEBUG_PRINT_ENABLE;

/*==================[macros and definitions]=================================*/

/* TEMP SENSOR => GPIO0 */

#define TEMPERATURESENSOR_SCUPORT		6
#define TEMPERATURESENSOR_SCUPIN		1
#define TEMPERATURESENSOR_GPIOPORT		3
#define TEMPERATURESENSOR_GPIOPIN		0


/*==================[internal data declaration]==============================*/
#define DEBUG_ENABLE

/** delay in milliseconds */
#define DELAY_MS 500

/*==================[internal functions declaration]=========================*/

static void initHardware(temperature_sensor *);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(temperature_sensor * tmpSensor)
{
	SystemCoreClockUpdate();
	Board_Init();
	tickConfig( 1 );
	debugPrintConfigUart( UART_USB, 115200 );
	owInit(tmpSensor);
}

/*==================[external functions definition]==========================*/

int main(void)
{
	int temp = 1;
	char str[20];
	temperature_sensor tmpSensor = { TEMPERATURESENSOR_SCUPORT, TEMPERATURESENSOR_SCUPIN, TEMPERATURESENSOR_GPIOPORT, TEMPERATURESENSOR_GPIOPIN };

	initHardware(&tmpSensor);

	while (1) {
		/* read DS18B20 temperature sensor */
		temp = owReadTemperature(&tmpSensor);
		sprintf(str, "temp:%d.%04d\r\n", temp >> 4, (temp & 0xF) * 625);
		debugPrintString(str);
		delay(DELAY_MS);
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
