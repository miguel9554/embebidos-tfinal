/*==================[inclusions]=============================================*/

#include "sapi.h"
#include "esp8266.h"
#include <string.h>

/*==================[macros and definitions]=================================*/

// Se deben definir los datos del nombre de la red y la contrasenia.
#define WIFI_NAME        "RompeMuros"     // Setear Red Wi-Fi
#define WIFI_PASS        "Juan2019" // Setear password

// El maximo tiempo que se espera una respuesta del modulo ESP8266
#define WIFI_MAX_DELAY 600000

#define BUFFER_LEN 6000

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

	char * command;
	char buffer[BUFFER_LEN];
	unsigned long buffer_len = BUFFER_LEN;

	// Inicializar la placa
	boardConfig ();

	// Configuramos el puerto serie USB
	uartConfig (UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 1 );

	// Creamos la estructura asociada al servidor http
	// http_server server = {WIFI_NAME, WIFI_PASS, WIFI_MAX_DELAY};

	initESP8266();

	command = "AT+CWMODE=1";

	sendATcommand(command);

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {
		if(readESP8266Data(buffer, buffer_len)){
			if (strcmp(buffer, command) == 0){
				stdioPrintf(UART_USB, "Recibimos el echo bien...\r\n");
			} else if (strcmp(buffer, "") == 0){
				stdioPrintf(UART_USB, "Recibimos el vacio bien...\r\n");
			} else if (strcmp(buffer, "OK") == 0){
				stdioPrintf(UART_USB, "Recibimos el OK bien...\r\n");
			}  else {
				stdioPrintf(UART_USB, "WTF! recibimos <%s>\r\n", buffer);
			}
		}
	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
