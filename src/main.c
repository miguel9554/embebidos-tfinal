/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "heater.h"
#include "ds18b20.h"
#include "level.h"
#include "valve.h"

/*==================[macros and definitions]=================================*/

#define WELCOME_MESSAGE "BIENVENIDO.\r\nPara comenzar, llene la olla con el nivel deseado, y presione TEC1.\r\nPara ver los parametros cargados, presione TEC2.\r\n"

// GPIOPORTS DE LOS SENSORES/ACTUADORES

#define OLLA1_HEATER_GPIOPORT 2
#define OLLA1_TEMPSENSOR_GPIOPORT 0
#define OLLA2_LEVELSENSOR_GPIOPORT 1
#define VALVE_GPIOPORT 3

/*==================[internal functions declaration]=========================*/

void initSystem(temperature_sensor *, heater *, valve *, level_sensor *);
void calentarOlla(temperature_sensor *, heater *, int);
void llenarOlla(valve *, level_sensor *);
void esperarMinutos(uint8_t minutos);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void initSystem(temperature_sensor * olla1_tempSensor, heater * olla1_calentador, valve *valve, level_sensor * olla2_level_sensor){

	// Inicializar la placa
	boardConfig();

	// Configuramos el puerto serie USB
	uartConfig(UART_USB, 115200);

	// Configuramos los ticks para que se den cada 1 milisegundo
	tickConfig( 50 );

	// Inicializamos los sensores/actuadores
	owInit(olla1_tempSensor, OLLA1_TEMPSENSOR_GPIOPORT);
	configHeater(olla1_calentador, OLLA1_HEATER_GPIOPORT);
	configLevelSensor(olla2_level_sensor, OLLA2_LEVELSENSOR_GPIOPORT);
	configValve(valve, VALVE_GPIOPORT);

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

void llenarOlla(valve * valve, level_sensor * olla2_level_sensor){

	bool level_reached = false;

	valveOpen(valve);
	while(!level_reached){
		level_reached = readLevelSensor(olla2_level_sensor);
	}
	valveClose(valve);

}

void esperarMinutos(uint8_t minutos){
	while(minutos){
		delay(60000);
		minutos--;
	}
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

	// VARIABLES VARIAS
	bool programReadyToStart = false;
	char str[100];

	// VARIABLES DE CONTROL
	float tempraturaDeseadaOlla1 = 30;
	uint8_t macerado_minutos_reposo = 2;

	// SENSORES Y ACTUADORES
	temperature_sensor olla1_tempSensor;
	heater olla1_calentador;
	level_sensor olla2_level_sensor;
	valve valve;

	// iniciamos el sistema
	initSystem(&olla1_tempSensor, &olla1_calentador, &valve, &olla2_level_sensor);

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
			stdioPrintf(UART_USB, "Tiempo de reposo para el macerado: %d minutos\r\n", macerado_minutos_reposo);


			delay(500);
		}
	}

	/*****************************************************************************/
	/********************************** ETAPA 1 **********************************/
	/*****************************************************************************/

	// etapa de calentado de olla 1

	// indicamos el estado
	gpioWrite(LEDR, 0);
	gpioWrite(LEDG, 1);
	gpioWrite(LED3, 1);
	stdioPrintf(UART_USB, "Empezando a calentar Olla 1\r\n");

	// calentamos la olla
	calentarOlla(&olla1_tempSensor, &olla1_calentador, tempraturaDeseadaOlla1);

	// indicamos que termino la etapa
	stdioPrintf(UART_USB, "Olla 1 calentada\r\n");
	gpioWrite(LED3, 0);

	/*****************************************************************************/
	/********************************** ETAPA 2 **********************************/
	/*****************************************************************************/

	// etapa de llenado de olla 2

	// indicamos el estado
	gpioWrite(LED2, 1);
	stdioPrintf(UART_USB, "Empezamos a llenar la olla 2\r\n");

	// llenamos la olla
	llenarOlla(&valve, &olla2_level_sensor);

	// indicamos que termino la etapa
	stdioPrintf(UART_USB, "Olla 2 llenada\r\n");
	gpioWrite(LED2, 0);

	/*****************************************************************************/
	/********************************** ETAPA 3 **********************************/
	/*****************************************************************************/

	// etapa de llenado de olla 2

	// indicamos el estado
	gpioWrite(LEDG, 0);
	gpioWrite(LEDR, 1);

	stdioPrintf(UART_USB, "Esperando a que se coloquen los granos y se revuelva.\r\nCuando se haya terminado, presionar TEC1 para continuar el proceso.\r\n");

	// esperamos a que presionen
	while (gpioRead(TEC1)){}

	stdioPrintf(UART_USB, "Etapa de reposo. Se dejara reposar por %d minutos", macerado_minutos_reposo);
	gpioWrite(LEDR, 0);
	gpioWrite(LEDG, 1);
	gpioWrite(LED3, 1);
	gpioWrite(LED2, 2);

	esperarMinutos(macerado_minutos_reposo);

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while(1) {

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/
