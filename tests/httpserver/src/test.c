/*==================[inclusions]=============================================*/

#include "sapi.h"
#include "httpserver.h"

/*==================[macros and definitions]=================================*/

// Se deben definir los datos del nombre de la red y la contrasenia.
#define WIFI_NAME        "RompeMuros"     // Setear Red Wi-Fi
#define WIFI_PASS        "Juan2019" // Setear password

// El maximo tiempo que se espera una respuesta del modulo ESP8266
#define WIFI_MAX_DELAY 60000

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

	char * ip_address;

	// Inicializar la placa
	boardConfig ();

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 1 );

	// Creamos la estructura asociada al servidor http
	http_server server = {WIFI_NAME, WIFI_PASS, WIFI_MAX_DELAY};

	// Configura la UART USB a 115200 baudios
	uartConfig (UART_USB, 115200);

	// Envia un mensaje de bienvenida.
	stdioPrintf(UART_USB, "\n\rBienvenido al servidor HTTP Esp8266 con EDU CIAA");
	stdioPrintf(UART_USB, "\n\rLa configuracion puede tardar hasta 1 minuto.");

	ip_address = configHTTPserver(&server);

	if (ip_address != NULL){
		stdioPrintf(UART_USB, "\n\rServidor HTTP configurado. IP: %s", ip_address);
		// Enciende LEDG indicando que el modulo esta configurado.
		gpioWrite(LEDG, TRUE);
	} else {
		stdioPrintf(UART_USB, "\n\rError al configurar servidor HTTP");
		// Enciende LEDR indicando que el modulo esta en error.
		gpioWrite(LEDR, TRUE);
	}

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
