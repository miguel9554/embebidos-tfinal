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

#ifndef APPLICATION_PORTSMAPPING_H_
#define APPLICATION_PORTSMAPPING_H_
/** @brief Brief for this header file.
 **
 ** Full description for this header file.
 **
 **/

/** \addtogroup groupName Group Name
 ** @{ */


/*==================[inclusions]=============================================*/

#include "sapi.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/* cada fila representa un gpio de la EDU-CIAA
 * las primeras dos columnas son scu_port y scu_pin
 * tercer y cuarta columna gpio port y pin
 * quinta columna función							 */

const uint8_t gpioPortsMapping[9][5] = {
		{6, 1, 3, 0, SCU_MODE_FUNC0},
		{6, 4, 3, 3, SCU_MODE_FUNC0},
		{6, 5, 3, 4, SCU_MODE_FUNC0},
		{6, 7, 5, 15, SCU_MODE_FUNC4},
		{6, 8, 5, 16, SCU_MODE_FUNC4},
		{6, 9, 3, 5, SCU_MODE_FUNC0},
		{6, 10, 3, 6, SCU_MODE_FUNC0},
		{6, 11, 3, 7, SCU_MODE_FUNC0},
		{6, 12, 3, 8, SCU_MODE_FUNC0},
};


/*==================[external functions declaration]=========================*/


/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */
/*==================[end of file]============================================*/


#endif /* APPLICATION_INC_ONEWIRE_H_ */
