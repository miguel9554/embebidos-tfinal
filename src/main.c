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

void calentarOlla(int, temperature_sensor *, heater *);
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
	tickConfig( 1 );

	// Inicializamos los sensores/actuadores

	owInit(olla1_tempSensor, OLLA1_TEMPSENSOR_GPIOPORT);
	configHeater(olla1_calentador, OLLA1_HEATER_GPIOPORT);
}

void calentarOlla(int temperaturaDeseada, temperature_sensor * tempSensor, heater * heater){

	int temp = 1;
	float fTemp = 1;
	float fTemperaturaDeseada = temperaturaDeseada;
	delay_t delayImprimirTemp;

	delayConfig(&delayImprimirTemp, 5000);

	temp = owReadTemperature(tempSensor);
	fTemp = (temp >> 4) + ((temp & 0xF) * 0.0625);

	heaterON(heater);
	// evitamos el glitch de 85
	while ((fTemperaturaDeseada > fTemp) && (fTemp != 85.0)){
		temp = owReadTemperature(tempSensor);
		fTemp = (temp >> 4) + ((temp & 0xF) * 0.0625);
		if (delayRead(&delayImprimirTemp)){
			stdioPrintf(UART_USB, "Temperatura de la olla 1: %f", fTemp);
		}
	}
	heaterOFF(heater);
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

	// VARIABLES VARIAS
	bool programReadyToStart = false;

	// VARIABLES DE CONTROL
	float tempraturaDeseadaOllaUno = 30;

	// SENSORES Y ACTUADORES
	temperature_sensor olla1_tempSensor;
	heater olla1_calentador;

	// iniciamos el sistema
	initSystem(&olla1_tempSensor, &olla1_calentador);

	stdioPrintf(UART_USB, WELCOME_MESSAGE);

	while(!programReadyToStart){
		if (!gpioRead(TEC1)){
			programReadyToStart = true;
		}
		if (!gpioRead(TEC2)){
			stdioPrintf(UART_USB, "Los parametros cargados son los siguientes\r\n");
			stdioPrintf(UART_USB, "Temperatura de la olla 1: %f\r\n", tempraturaDeseadaOllaUno);
			delay(500);
		}
	}

	stdioPrintf(UART_USB, "Empezando a calentar Olla 1\r\n");

	// esperamos a que se llegue a la temperatura deseada
	calentarOlla(tempraturaDeseadaOllaUno, &olla1_tempSensor, &olla1_calentador);

	stdioPrintf(UART_USB, "Olla 1 calentada\r\n");

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
