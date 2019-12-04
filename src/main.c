/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"

/*==================[macros and definitions]=================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

	// Inicializar la placa
	boardConfig();

	// Configuramos el puerto serie USB
	uartConfig(UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 1 );

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {
		gpioWrite(LED2, 0);
		gpioWrite(LED1, 1);
		delay(5000);
		gpioWrite(LED1, 0);
		gpioWrite(LED2, 1);
		delay(5000);
	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
