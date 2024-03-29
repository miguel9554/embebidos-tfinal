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

#include "valve.h"
#include "sapi.h"
#include "portsMapping.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void configValve(valve * valve, uint8_t eduCIAAgpio){

	valve->scu_port = gpioPortsMapping[eduCIAAgpio][0];
	valve->scu_pin = gpioPortsMapping[eduCIAAgpio][1];
	valve->gpio_port = gpioPortsMapping[eduCIAAgpio][2];
	valve->gpio_pin = gpioPortsMapping[eduCIAAgpio][3];
	valve->scu_function = gpioPortsMapping[eduCIAAgpio][4];

	Chip_SCU_PinMux(
					valve->scu_port,
					valve->scu_pin,
		            SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
					valve->scu_function
		         );
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, valve->gpio_port, valve->gpio_pin);

	valveClose(valve);
}

void valveOpen(valve * valve){
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, valve->gpio_port, valve->gpio_pin);
}

void valveClose(valve * valve){
	Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, valve->gpio_port, valve->gpio_pin);
}

/** @} doxygen end group definition */
	/*==================[end of file]============================================*/
