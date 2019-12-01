/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
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

/*
 * Date: 2016-04-26
 */

/*==================[inclusions]=============================================*/

#include "ds18b20.h"
#include "heater.h"
#include "sapi.h"         // <= sAPI header
DEBUG_PRINT_ENABLE;

/*==================[macros and definitions]=================================*/

/* HEATER -> GPIO2 */

#define HEATER_PINNAMEPORT	6
#define HEATER_PINNAMEPIN	5
#define HEATER_GPIOPORT		3
#define HEATER_GPIOPIN		4


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void heaterControl( Heater * heater){

	int temp;
	float fTemp;
	char str[20];

	temp = owReadTemperature();
	fTemp = (temp >> 4) + ((temp & 0xF) * 0.0625);
	sprintf(str, "temp:%f\r\n", fTemp);
	debugPrintString(str);

	if (fTemp > 26.5){
		heaterOFF(heater);
	} else{
		heaterON(heater);
	}

	return;
}


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
	SystemCoreClockUpdate();
	Board_Init();
	SysTick_Config(SystemCoreClock / 1000);
	debugPrintConfigUart( UART_USB, 115200 );
	owInit();
	Heater heater = { HEATER_PINNAMEPORT, HEATER_PINNAMEPIN, HEATER_GPIOPORT, HEATER_GPIOPIN};
	configHeater(&heater);

   /* Conteo de ticks cada 10 milisegundos, así vamos a estar activando o no medio ciclo de la señal. */
   tickConfig( 10 );

   /* Definimos como tickhook al control de la temperatura, se va a ejecutar cada 10 milis. */
   tickCallbackSet( heaterControl, &heater);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
