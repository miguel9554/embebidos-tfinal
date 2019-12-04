/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "heater.h"
#include "ds18b20.h"

/*==================[macros and definitions]=================================*/

#define WELCOME_MESSAGE "BIENVENIDO.\r\nPara comenzar, llene la olla con el nivel deseado, y presione TEC1.\r\nPara ver los parametros cargados, presione TEC2.\r\n"

// GPIOPORTS DE LOS SENSORES/ACTUADORES

#define OLLA1_HEATER_GPIOPORT 2
#define OLLA1_TEMPSENSOR_GPIOPORT 0

/*==================[internal functions declaration]=========================*/

void calentarOlla(temperature_sensor *, heater *, int);
void initSystem(temperature_sensor *, heater *);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void initSystem(temperature_sensor * olla1_tempSensor, heater * olla1_calentador){

	// Inicializar la placa
	boardConfig();

	// Configuramos el puerto serie USB
	uartConfig(UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 50 );

	// Inicializamos los sensores/actuadores

	owInit(olla1_tempSensor, OLLA1_TEMPSENSOR_GPIOPORT);
	configHeater(olla1_calentador, OLLA1_HEATER_GPIOPORT);
}

void calentarOlla(temperature_sensor * tempSensor, heater * heater, int temperaturaDeseada){

	char str[100];
	int temp = 1;
	float fTemp = 1;
	float fTemperaturaDeseada = temperaturaDeseada;
	int glitchTemperature = 0x550;
	delay_t delayImprimirTemp;

	delayConfig(&delayImprimirTemp, 5000);

	temp = owReadTemperature(tempSensor);
	fTemp = (temp >> 4) + ((temp & 0xF) * 0.0625);
	sprintf(str, "Temperatura de la olla 1: %.2f\r\n", fTemp);
	stdioPrintf(UART_USB, str);

	heaterON(heater);
	// evitamos el glitch de 85
	while ((fTemperaturaDeseada > fTemp) && (temp != glitchTemperature)){
		temp = owReadTemperature(tempSensor);
		fTemp = (temp >> 4) + ((temp & 0xF) * 0.0625);
		if (delayRead(&delayImprimirTemp)){
			sprintf(str, "Temperatura de la olla 1: %.2f\r\n", fTemp);
			stdioPrintf(UART_USB, str);
		}
	}
	heaterOFF(heater);
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

	// VARIABLES VARIAS
	bool programReadyToStart = false;
	char str[100];

	// VARIABLES DE CONTROL
	float tempraturaDeseadaOlla1 = 30;

	// SENSORES Y ACTUADORES
	temperature_sensor olla1_tempSensor;
	heater olla1_calentador;


	// iniciamos el sistema
	initSystem(&olla1_tempSensor, &olla1_calentador);

	stdioPrintf(UART_USB, WELCOME_MESSAGE);

	// LED de status, ciclo no empezado
	gpioWrite(LEDR, 1);

	while(!programReadyToStart){
		if (!gpioRead(TEC1)){
			programReadyToStart = true;
		}
		if (!gpioRead(TEC2)){
			stdioPrintf(UART_USB, "Los parametros cargados son los siguientes\r\n");

			// Temperatura olla 1
			sprintf(str, "Temperatura objetivo de la olla 1: %.2f\r\n", tempraturaDeseadaOlla1);
			stdioPrintf(UART_USB, str);

			delay(500);
		}
	}

	// indicamos que el proceso empezo, y que estamos en la etapa 1
	gpioWrite(LEDR, 0);
	gpioWrite(LEDG, 1);
	gpioWrite(LED3, 1);

	stdioPrintf(UART_USB, "Empezando a calentar Olla 1\r\n");

	// calentamos la olla 1
	calentarOlla(&olla1_tempSensor, &olla1_calentador, tempraturaDeseadaOlla1);

	stdioPrintf(UART_USB, "Olla 1 calentada\r\n");

	// led de etapa 2
	// llenarOlla();

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
