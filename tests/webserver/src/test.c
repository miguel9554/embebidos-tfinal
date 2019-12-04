/*==================[inclusions]=============================================*/

#include "sapi.h"
#include "esp8266.h"
#include "webserver.h"
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

	char data[BUFFER_LEN];
	float tempraturaDeseadaOlla1 = 30;						// /temp/olla/1/<valor>
	uint8_t macerado_minutos_reposo = 2;					// /minutos/reposo/macerado/<valor>

	float tempPrimerPerfilTemperatura = 30;					// /temp/perfil/1/<valor>
	uint8_t minutosReposoPrimerPerfilTemperatura = 2;		// /minutos/reposo/perfil/1/<valor>

	float tempSegundoPerfilTemperatura = 30;				// /temp/perfil/2/<valor>
	uint8_t minutosReposoSegundoPerfilTemperatura = 2;		// /minutos/reposo/perfil/2/<valor>

	float tempTercerPerfilTemperatura = 30;					// /temp/perfil/2/<valor>
	uint8_t minutosReposoTercerPerfilTemperatura = 2;		// /minutos/reposo/perfil/2/<valor>

	// Inicializar la placa
	boardConfig();

	// Configuramos el puerto serie USB
	uartConfig(UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 50 );

	if (configWebServer()){
		gpioWrite(LEDG, 1);
	} else{
		gpioWrite(LEDR, 1);
		while(1){}
	}

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {
		if (receiveData(data)){
			assignVariablesData(data, &tempraturaDeseadaOlla1, &macerado_minutos_reposo, &tempPrimerPerfilTemperatura,
						&minutosReposoPrimerPerfilTemperatura, &tempSegundoPerfilTemperatura, &minutosReposoSegundoPerfilTemperatura,
						&tempTercerPerfilTemperatura, &minutosReposoTercerPerfilTemperatura);
		}
	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
