/*
 * test.c
 *
 *  Created on: Dec 4, 2019
 *      Author: miguel
 */

#include "sapi.h"

void esperarMinutos(uint8_t minutos){
	stdioPrintf(UART_USB, "Empezamos a esperar\r\n");
	while(minutos){
		delay(60000);
		minutos--;
		stdioPrintf(UART_USB, "Faltan %d minutos!\r\n", minutos);
	}
}

int main (void){

	uint8_t macerado_minutos_reposo = 2;

	// Inicializar la placa
	boardConfig();

	// Configuramos el puerto serie USB
	uartConfig(UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 50 );

	while(1){
		stdioPrintf(UART_USB, "Vamos a esperar %d minutos\r\n", macerado_minutos_reposo);
		esperarMinutos(macerado_minutos_reposo);
		stdioPrintf(UART_USB, "Pasaron %d minutos!\r\n", macerado_minutos_reposo);
	}
}
