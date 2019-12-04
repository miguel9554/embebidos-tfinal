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

void assignVariablesData(char *, float *, uint8_t *, float *, uint8_t *, float *, uint8_t *, float *, uint8_t *);
int StrToInt(const char* );
bool matchGET(char *, char*, uint8_t, char *);

/*==================[internal data definition]===============================*/
char GETstring_tempraturaDeseadaOlla1[]  = "/temp/olla/1/"; // 13
char GETstring_macerado_minutos_reposo[] = "/minutos/reposo/macerado/"; // 25

char GETstring_tempPrimerPerfilTemperatura[] = "/temp/perfil/1/"; // 15
char GETstring_minutosReposoPrimerPerfilTemperatura[] = "/minutos/reposo/perfil/1/";// 25

char GETstring_tempSegundoPerfilTemperatura[] = "/temp/perfil/2/"; // 15
char GETstring_minutosReposoSegundoPerfilTemperatura[] = "/minutos/reposo/perfil/2/"; // 25

char GETstring_tempTercerPerfilTemperatura[] = "/temp/perfil/2/"; // 15
char GETstring_minutosReposoTercerPerfilTemperatura[] = "/minutos/reposo/perfil/2/"; // 25

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void assignVariablesData(char * data, float * tempraturaDeseadaOlla1, uint8_t * macerado_minutos_reposo, float * tempPrimerPerfilTemperatura,
		uint8_t * minutosReposoPrimerPerfilTemperatura, float * tempSegundoPerfilTemperatura, uint8_t * minutosReposoSegundoPerfilTemperatura,
		float * tempTercerPerfilTemperatura, uint8_t * minutosReposoTercerPerfilTemperatura){

	char result[50];
	int tmp;
	if(matchGET(GETstring_tempraturaDeseadaOlla1, data, strlen(GETstring_tempraturaDeseadaOlla1), result)){
		stdioPrintf(UART_USB, "Recibimos una temperatura para la olla 1\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*tempraturaDeseadaOlla1 = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_macerado_minutos_reposo, data, strlen(GETstring_macerado_minutos_reposo), result)){
		stdioPrintf(UART_USB, "Recibimos los minutos de reposo del macerado\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*macerado_minutos_reposo = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_tempPrimerPerfilTemperatura, data, strlen(GETstring_tempPrimerPerfilTemperatura), result)){
		stdioPrintf(UART_USB, "Recibimos la temperatura del primer perfil\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*tempPrimerPerfilTemperatura = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_minutosReposoPrimerPerfilTemperatura, data, strlen(GETstring_minutosReposoPrimerPerfilTemperatura), result)){
		stdioPrintf(UART_USB, "Recibimos los minutos de reposo del primer perfil\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*minutosReposoPrimerPerfilTemperatura = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_tempSegundoPerfilTemperatura, data, strlen(GETstring_tempSegundoPerfilTemperatura), result)){
		stdioPrintf(UART_USB, "Recibimos la temperatura del segundo perfil\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*tempSegundoPerfilTemperatura = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_minutosReposoSegundoPerfilTemperatura, data, strlen(GETstring_minutosReposoSegundoPerfilTemperatura), result)){
		stdioPrintf(UART_USB, "Recibimos los minutos de reposo del segundo perfil\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*minutosReposoSegundoPerfilTemperatura = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_tempTercerPerfilTemperatura, data, strlen(GETstring_tempTercerPerfilTemperatura), result)){
		stdioPrintf(UART_USB, "Recibimos la temperatura del tercer perfil\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*tempTercerPerfilTemperatura = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else if(matchGET(GETstring_minutosReposoTercerPerfilTemperatura, data, strlen(GETstring_minutosReposoTercerPerfilTemperatura), result)){
		stdioPrintf(UART_USB, "Recibimos los minutos de reposo del tercer perfil\r\n");
		tmp = StrToInt(result);
		if(tmp){
			*minutosReposoTercerPerfilTemperatura = tmp;
			stdioPrintf(UART_USB, "Dato OK\r\n");
		} else {
			stdioPrintf(UART_USB, "Dato mal...\r\n");
		}
	}else{
		stdioPrintf(UART_USB, "No matchea con nada\r\n");
	}
}

bool matchGET(char * pattern, char * data, uint8_t length, char * result){
	int j = 0;
	int i = 0;
	for (i = 0; i < length; i++){
		if (*(pattern + i) != *(data + i)){
			result = NULL;
			return false;
		}
	}
	while( *(data + i + j)){
		*(result + j) = *(data + i + j);
		j++;
	}
	*(result + j) = '\0';
	return true;
}

// Convert standard null-terminated string to an integer
// - Skips leading whitespaces.
// - Skips trailing whitespaces.
// - Allows for one, optional +/- sign at the front.
// - Returns zero if any non-+/-, non-numeric, non-space character is encountered.
// - Returns zero if digits are separated by spaces (eg "123  45")
// - Range is checked against Overflow/Underflow (INT_MAX / INT_MIN), and returns 0.
int StrToInt(const char* s)
{
    char* w;

    do { // Skip any leading whitespace
        for(w=" \t\n\v\f\r"; *w && *s != *w; ++w) ;
        if (*s == *w) ++s; else break;
    } while(*s);

    int sign = 1;
    if ('-' == *s) sign = -1;
    if ('+' == *s || '-' == *s) ++s;

    long long i=0;
    while('0' <= *s && *s <= '9')
    {
        i = 10*i + *s++ - '0';
    }

    while (*s) // Skip any trailing whitespace
    {
        for(w=" \t\n\v\f\r"; *w && *s != *w; ++w) ;
        if (*w && *s == *w) ++s; else break;
    }

    return (int)(!*s*sign*i);
}

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
