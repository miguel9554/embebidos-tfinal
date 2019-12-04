/* Copyright 2015, Pablo Ridolfi
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

/** @brief Brief for this file.
 **
 **/

/** \addtogroup groupName Group Name
 ** @{ */

/*==================[inclusions]=============================================*/

#include "pump.h"
#include "sapi.h"
#include "portsMapping.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void configPump(pump * pump, uint8_t eduCIAAgpio){

	pump->scu_port = gpioPortsMapping[eduCIAAgpio][0];
	pump->scu_pin = gpioPortsMapping[eduCIAAgpio][1];
	pump->gpio_port = gpioPortsMapping[eduCIAAgpio][2];
	pump->gpio_pin = gpioPortsMapping[eduCIAAgpio][3];
	pump->scu_function = gpioPortsMapping[eduCIAAgpio][4];

	Chip_SCU_PinMux(
					pump->scu_port,
					pump->scu_pin,
		            SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
					pump->scu_function
		         );
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, pump->gpio_port, pump->gpio_pin);

	pumpOFF(pump);
}

void pumpOFF(pump * pump){
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, pump->gpio_port, pump->gpio_pin);
}

void pumpON(pump * pump){
	Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, pump->gpio_port, pump->gpio_pin);
}

/** @} doxygen end group definition */
	/*==================[end of file]============================================*/
