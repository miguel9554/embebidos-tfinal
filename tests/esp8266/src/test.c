/*==================[inclusions]=============================================*/

#include "sapi.h"
#include "esp8266.h"

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

	// Inicializar la placa
	boardConfig ();

	// Configuramos el puerto serie USB
	uartConfig (UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 1 );

	// Creamos la estructura asociada al servidor http
	// http_server server = {WIFI_NAME, WIFI_PASS, WIFI_MAX_DELAY};

	initESP8266();

	command = "AT+RESTORE";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Reseteo OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos reseteando\r\n");
	}

	command = "AT+CWMODE=1";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteado como station OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando como station\r\n");
	}

	command= "AT+CWJAP=\"RompeMuros\",\"Juan2019\"";

	// esto manda
	// echo
	// wifi connected
	// got ip
	// vacio
	// ok

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Conecatdo a la red OK!\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos conectando a la red\r\n");
	}

	command = "AT+CIPMUX=1";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteadas multiples conexiones OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando multiples conexiones\r\n");
	}

	command = "AT+CIPSERVER=1,80";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteado servidor en puerto 80 OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando servidor en puerto 80\r\n");
	}

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {
	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
