/* Copyright 2015, Pablo Ridolfi
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** @brief Brief for this file.
 **
 **/

/** \addtogroup groupName Group Name
 ** @{ */

/*==================[inclusions]=============================================*/

#include "esp8266.h"
#include "sapi.h"
#include <string.h>

/*==================[macros and definitions]=================================*/

#define BUFFER_LEN 6000
#define ESP8266_UART UART_232
#define ESP_BAUDRATE 115200
// delay de 10 segundos por comando
#define MAX_DELAY 10000

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

bool readESP8266Data(char * , unsigned long );
bool sendATcommand(char * , char * );
bool getIPadress(char * );

/*==================[internal data definition]===============================*/

typedef struct{
	bool active;
	char id;
} connection;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

bool readESP8266Data(char * buffer, unsigned long buffer_len){

	uint8_t dato  = 0;
	unsigned long i = 0;


	// buffer + i es la posición a la que vamos a escribir, es decir, una posición después de la última con algo escrito
	// buffer + i - 1 y buffer + i - 2 son, entonces, los últimos dos elementos
	// vamos guardando en el buffer hasta que estos sean \r y \n, lo que indica que el comando terminó

	while( ! ( *(buffer + i - 1) == 10 && *(buffer + i - 2) == 13) ){
		if(uartReadByte( UART_232, &dato )){
			// si se lleno el buffer, lo apuntamos a NULL y devolvemos false
			if (i > buffer_len){
				buffer = NULL;
				return false;
			}
			// si no, escribimos el dato leído
			*(buffer + i) = dato;
			i++;
		}
	}

	*(buffer + i - 2) = '\0';

	return true;

}

bool sendATcommand(char * command, char * expected_response){

	char buffer[BUFFER_LEN];
	unsigned long buffer_len = BUFFER_LEN;
	bool communication_finnished = false;
	bool communication_succesful = false;
	delay_t max_wait_time;

	delayConfig(&max_wait_time, MAX_DELAY);

	stdioPrintf(ESP8266_UART, "%s\r\n", command);

	// mandamos el comando, y esperamos la respuesta esperada o que pasen 10 segundos
	while (! (communication_finnished)){
		if(readESP8266Data(buffer, buffer_len)){
			if (strcmp(buffer, expected_response) == 0){
				//stdioPrintf(UART_USB, "Recibimos el OK bien...\r\n");
				communication_finnished = true;
				communication_succesful = true;
			}
		}
		if (delayRead(&max_wait_time)){
			communication_finnished = true;
			communication_succesful = false;
		}
	}

	return communication_succesful;
}

bool getIPadress(char * ip){

	char command[] = "AT+CIFSR";
	char expected_response[] = "OK";
	char buffer[BUFFER_LEN];
	unsigned long buffer_len = BUFFER_LEN;
	bool communication_finnished = false;
	bool communication_succesful = false;
	unsigned long i = 0;
	delay_t max_wait_time;

	delayConfig(&max_wait_time, MAX_DELAY);

	stdioPrintf(ESP8266_UART, "%s\r\n", command);
	//stdioPrintf(UART_USB, "Buscando IP\r\n");

	// mandamos el comando, y esperamos la respuesta esperada o que pasen 10 segundos
	while (! (communication_finnished)){
		if(readESP8266Data(buffer, buffer_len)){
			if (strcmp(buffer, expected_response) == 0){
				communication_finnished = true;
				communication_succesful = true;
			}
			if (buffer[0] == '+' && buffer[1] == 'C' && buffer[2] == 'I' && buffer[3] == 'F' && buffer[4] == 'S' && buffer[5] == 'R'
					 && buffer[6] == ':' && buffer[7] == 'S' && buffer[8] == 'T' && buffer[9] == 'A' && buffer[10] == 'I' && buffer[11] == 'P'){
				//stdioPrintf(UART_USB, "Encontramos el mensaje de IP\r\n");
				while( * (buffer + 14 + i) != '"'){
					* (ip + i) = * (buffer + 14 + i);
					i++;
				}
				* (ip + i) = '\0';
			}
		}
		if (delayRead(&max_wait_time)){
			communication_finnished = true;
			communication_succesful = false;
		}
	}

	return communication_succesful;
}

bool readIPdata(char * data){

	char buffer[BUFFER_LEN];
	unsigned long buffer_len = BUFFER_LEN;
	bool communication_finnished = false;
	bool communication_succesful = false;
	bool data_readed = false;
	unsigned long i = 0;
	delay_t max_wait_time;

	delayConfig(&max_wait_time, MAX_DELAY);

	stdioPrintf(ESP8266_UART, "AT+CIPRECVDATA=0,200\r\n");
	//stdioPrintf(UART_USB, "Buscando IP\r\n");

	// mandamos el comando, y esperamos la respuesta esperada o que pasen 10 segundos
	while (! (communication_finnished)){
		if(readESP8266Data(buffer, buffer_len)){
			if (strcmp(buffer, "OK") == 0){
				communication_finnished = true;
				communication_succesful = true;
			}
			// +CIPRECVDATA:<actual_len>,<data>
			if (buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T'){
				while( * (buffer + i)){
					* (data + i) = * (buffer + i);
					i++;
				}
				* (data + i) = '\0';
				data_readed = true;
			}
		}
		if (delayRead(&max_wait_time)){
			communication_finnished = true;
			communication_succesful = false;
		}
	}

	return communication_succesful && data_readed;
}


/*==================[external functions definition]==========================*/

bool configWebServer(){

	char * command;
	char ip_address[BUFFER_LEN];

	uartConfig (ESP8266_UART, ESP_BAUDRATE);

	command = "AT+RESTORE";

	if(sendATcommand(command, "ready")){
		stdioPrintf(UART_USB, "Reseteo OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos reseteando\r\n");
		return false;
	}

	command = "AT+CWMODE=1";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteado como station OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando como station\r\n");
		return false;
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
		return false;
	}

	command = "AT+CIPMUX=1";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteadas multiples conexiones OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando multiples conexiones\r\n");
		return false;
	}

	command = "AT+CIPRECVMODE=1";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteadas modo recibo pasivo OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando modo recibo pasivo\r\n");
		return false;
	}

	command = "AT+CIPSERVERMAXCONN=1";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteadas conexiones maximas igual a 1 OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando conexiones maximas igual a 1 \r\n");
		return false;
	}

	command = "AT+CIPSERVER=1,80";

	if(sendATcommand(command, "OK")){
		stdioPrintf(UART_USB, "Seteado servidor en puerto 80 OK\r\n");
	} else {
		stdioPrintf(UART_USB, "ERROR: Fallamos seteando servidor en puerto 80\r\n");
		return false;
	}

	if (getIPadress(ip_address)){
		stdioPrintf(UART_USB, "Servidor funcionando en puerto 80, ip: %s\r\n", ip_address);
	} else {
		stdioPrintf(UART_USB, "Fallamos obteniendo la IP...%s\r\n");
		return false;
	}

	return true;
}

bool receiveData(char * data){

	char buffer[BUFFER_LEN];
	unsigned long buffer_len = BUFFER_LEN;

	if(readESP8266Data(buffer, buffer_len)){
		if(buffer[1] == ',' && buffer[2] == 'C' && buffer[3] == 'O' && buffer[4] == 'N' && buffer[5] == 'N' && buffer[6] == 'E' && buffer[7] == 'C'
				 && buffer[8] == 'T'){
			stdioPrintf(UART_USB, "Nueva conexion: %c\r\n", buffer[0]);
		} else if (buffer[0] == '+' && buffer[1] == 'I' && buffer[2] == 'P' && buffer[3] == 'D'){
			stdioPrintf(UART_USB, "Recibimos datos\r\n");

			if (readIPdata(data)){
				//"<!DOCTYPE HTML><html>Recibido</html>"
				if(sendATcommand("AT+CIPCLOSE=0", "OK")){
					stdioPrintf(UART_USB, "Datos leidos, cerramos la conexion\r\n");
					return true;
				} else {
					stdioPrintf(UART_USB, "ERROR: Fallamos cerrando la conexion\r\n");
					return false;
				}
			} else{
				stdioPrintf(UART_USB, "ERROR: Fallamos leyendo los datos\r\n");
				return false;
			}
		}
	}

	return false;
}


/** @} doxygen end group definition */
	/*==================[end of file]============================================*/
