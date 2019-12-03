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

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void initESP8266(){
	uartConfig (ESP8266_UART, ESP_BAUDRATE);
}

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

	stdioPrintf(ESP8266_UART, "%s\r\n", command);

	while (! (communication_finnished)){
		if(readESP8266Data(buffer, buffer_len)){
			if (strcmp(buffer, command) == 0){
				// stdioPrintf(UART_USB, "Recibimos el echo bien...\r\n");
			} else if (strcmp(buffer, "") == 0){
				// stdioPrintf(UART_USB, "Recibimos el vacio bien...\r\n");
			} else if (strcmp(buffer, expected_response) == 0){
				//stdioPrintf(UART_USB, "Recibimos el OK bien...\r\n");
				communication_finnished = true;
				communication_succesful = true;
			}  else {
				communication_finnished = true;
				communication_succesful = false;
			}
		}
	}

	return communication_succesful;
}


/** @} doxygen end group definition */
	/*==================[end of file]============================================*/
